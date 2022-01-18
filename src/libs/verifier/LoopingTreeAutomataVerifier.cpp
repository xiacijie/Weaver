#include "LoopingTreeAutomataVerifier.h"
#include "SMTInterpol.h"
#include "MathSAT.h"
#include "Yices.h"
#include "ProofAutomata.h"
#include "Config.h"
#include "Timer.h"


using namespace weaver;
using namespace std;
using namespace util;

extern Config config;
extern Logger logger;

bool LoopingTreeAutomataVerifier::verify() {

    logger.info("Start verifying with Looping Tree Automata...");

    bool correct = false;

    Program* program = _program;
    NFA* cfg = &program->getCFG();

    Timer timer;

    while (true) {
        logger.info("\n");
        logger.info(getRefinementRoundHeader());
        logger.info(getProofSizeHeader());

        logger.info(getDeterProofHeader());
        timer.start();
        DFA* DProof = _proof.NFAToDFA(program->getAlphabet());
        _proofDeterTime += timer.stop();

        logger.debug("# Current Proof DFA:");
        logger.debug(DProof->toString(_program->getAlphabet()));

        logger.info(getErrorTraceHeader());
        
        timer.start();
        set<Trace> errorTraceSet;
        if (config.antiChain) {
            errorTraceSet = proofCheckWithAntiChains(cfg, DProof);
        }
        else {
            errorTraceSet = proofCheck(cfg, DProof);
        }
        _proofCheckingTime += timer.stop();

        delete DProof;

        if (!errorTraceSet.empty()) {

            // currently we only choose the first counterexample from a set of error traces
            const auto& trace = *errorTraceSet.begin();
                logger.info(getErrorTraceString(trace));

                // This trace must be non-empty
                logger.info(getGenInterpolHeader());

                timer.start();
                Interpolants interpols = _interpolSolver->generateInterpols(trace);
                _interpolGenTime += timer.stop();

                if (interpols.empty()) {
                    logger.info("\n");
                    logger.warn(getVerificationFailureInfo(trace));
                    break;
                }

                logger.info(getInterpolantsString(interpols));

                logger.info(getGrowProofHeader());

                timer.start();
                _proof.extend(interpols, program->getAlphabet());
                _proofExtendTime += timer.stop();

                _refinementRound++;

        }
        else { // the program is correct
            logger.info("\n");
            logger.info(getVerificationSucessData());
            correct = true;
            break;
        }
    }

    return correct;
}


set<Trace> LoopingTreeAutomataVerifier::proofCheck(NFA* cfg, DFA* proof) {

    map<IntersectionState, map<Statement*, IntersectionState>> inactivityProof;

    set<IntersectionState> inactiveStates;
    bool notFixed = true;

    // first we compute the initial set of inactive states
    for (const auto& s1: cfg->getAcceptStates()) {
        for (const auto& s2: proof->getStates()) {
            if (!proof->isAcceptState(s2)) {
                LTAState ltaState(s1, false, {});
                IntersectionState intersectionState(ltaState, s2);
                inactiveStates.insert(intersectionState);
            }
        }
    }

    // set the reduction ordering R
    // The ordering uses the partition optimization
    // Note that the main thread is dependent with any other threads
    vector<const unordered_set<Statement*>*> R;
    R.reserve(_program->getTotalNumThreads());
    for (int i = 1; i < _program->getTotalNumThreads(); i ++) {
        R.push_back(&_program->getStatementsByThread(i));
    }

    //all the sleep sets
    set<set<Statement*>> sleepSetSets;
    set<Statement*> possibleStatementsInSleepSet;
    const auto& independenceRelation = _program->getIndependenceRelation();

    for (const auto& it: independenceRelation) {
        possibleStatementsInSleepSet.insert(it.second.begin(), it.second.end());
    }

    alphabetPowerSetGenerationHelper(possibleStatementsInSleepSet.begin(), possibleStatementsInSleepSet, {}, sleepSetSets);

    // Then we compute backward, enlarging the inactive states until we find we initial state in it
    // or the set does not grow
    while (notFixed) {

        notFixed = false;
        for (const auto& s1 : cfg->getStates()) {

            // This state in the CFG has no transition
            if (!cfg->hasTransitionFrom(s1)) {
                continue;
            }

            for (const auto& sleepSet: sleepSetSets) {
                for (const auto& s2: proof->getStates()) {
                    LTAState currentLTAState(s1, false, sleepSet);
                    IntersectionState currentIntersectionState(currentLTAState, s2);

                    // already in the inactive states
                    if (inactiveStates.find(currentIntersectionState) != inactiveStates.end()) {
                        continue;
                    }

                    bool forAllOrderingsExistOneTransitionIntoInactiveState = true;
                    sort(R.begin(), R.end());
                    do {

                        bool existOneTransitionToInactiveState = false;
                        for (const auto&t : cfg->getTransitions(s1)) {
                            Statement* stmt = t.first;

                            if (sleepSet.find(stmt) != sleepSet.end()) {
                                continue;
                            }

                            const auto& targetStates = t.second;
                            for (const auto& targetState: targetStates) {
                                set<Statement*> nextSleepSet;
                                uint32_t nextProofState;

                                if (stmt == nullptr) {
                                    nextSleepSet.insert(sleepSet.begin(), sleepSet.end());
                                    nextProofState = currentIntersectionState.second;
                                }
                                else {
                                    nextProofState = proof->getTargetState(currentIntersectionState.second, stmt);

                                    set<Statement*> R_a;
                                    for (const auto & set : R) {
                                        if (set->find(stmt) != set->end()) {
                                            break;
                                        }
                                        else {
                                            R_a.insert(set->begin(), set->end());
                                        }
                                    }

                                    set<Statement*> S_union_R_a;
                                    setUnion(sleepSet, R_a, S_union_R_a);
                                
                                    set<Statement*> S_union_R_a_minus_D_a;
                                    const auto& D_a_u = _program->getDependentStatements(stmt);
                                    set<Statement*> D_a(D_a_u.begin(), D_a_u.end());
                                    setDifference(S_union_R_a, D_a, S_union_R_a_minus_D_a);

                                    nextSleepSet.insert(S_union_R_a_minus_D_a.begin(), S_union_R_a_minus_D_a.end());
                                }

                              

                                LTAState nextLTAState(targetState, false, nextSleepSet);
                                IntersectionState nextIntersectionState(nextLTAState, nextProofState);

                                // if any state transits into an inactive state
                                const auto& it = inactiveStates.find(nextIntersectionState);
                                if (it != inactiveStates.end()) {
                                    existOneTransitionToInactiveState = true;
                                    inactivityProof[currentIntersectionState][stmt] = nextIntersectionState;
                                    break;
                                }
                            }

                            if (existOneTransitionToInactiveState) {
                                break;
                            }
                        }

                        if (!existOneTransitionToInactiveState) {
                            forAllOrderingsExistOneTransitionIntoInactiveState = false;
                            break;
                        }

                    } while (next_permutation(R.begin(), R.end()));

                    if (forAllOrderingsExistOneTransitionIntoInactiveState) {

                        // initial state is inactive
                        if (cfg->isStartState(s1) && sleepSet.empty() && proof->isStartState(s2)) {
                            LTAState initialLTAState(s1, false, {});
                            IntersectionState initialIntersectionState(initialLTAState, s2);

                            return getCounterExamples(inactivityProof, initialIntersectionState);
                        }

                        notFixed = true;

                        inactiveStates.insert(currentIntersectionState);
                    }
                    else {
                        const auto& it = inactivityProof.find(currentIntersectionState);
                        if (it != inactivityProof.end()) {
                            inactivityProof.erase(it);
                        }
                    }

                }
            }
        }

    }

    return {};
}

string LoopingTreeAutomataVerifier::inactivityProofToString(
        const map<IntersectionState, map<Statement *, IntersectionState>> &inactivityProof) {
    stringstream ss;

    ss << "# Inactivity Proof: " << endl;
    for (const auto& it : inactivityProof) {
        auto f = it.first;

        ss <<"From: (" << get<0>(f.first) << "," << f.second << ")" << endl;

        for (const auto& it2 : it.second) {

            string stmt = "NULL";
            if (it2.first != nullptr)
                stmt = it2.first->toString();

            ss << "    " << "takes: " << stmt << " ";
            auto t = it2.second;
            ss << " to (" << get<0>(t.first) << "," << t.second << ")" << endl;
        }
    }

    return ss.str();
}

string LoopingTreeAutomataVerifier::inactivityProofToString(
        const map<T, map<set<Statement *>, map<Statement *, IntersectionState>>> &inactivityProof) {

    stringstream ss;
    ss << "# Inactivity Proof: " << endl;
    for (const auto &it : inactivityProof) {
        auto f = it.first;
        ss << "From: (" << get<0>(f) << "," << get<2>(f) << ")" << endl;
        for (const auto& it2: it.second) {
            for (const auto& it3 : it2.second) {
                string stmt = "NULL";

                if (it3.first != nullptr) {
                    stmt = it3.first->toString();
                }

                ss << "    " << "takes: " << stmt << " ";
                auto t = it3.second;

                ss << " to (" << get<0>(t.first) << "," << t.second <<")"<< endl;
            }
        }
    }
    return ss.str();
}

set<Trace> LoopingTreeAutomataVerifier::getCounterExamples(
        const map <IntersectionState, map<Statement *, IntersectionState>> &inactivityProof,
        IntersectionState& initialState) {

    logger.debug(inactivityProofToString(inactivityProof));

    set<Trace> counterExamples;
    queue<pair<IntersectionState, Trace>> q;

    q.push(make_pair(initialState, Trace({})));

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        const auto& currentState = current.first;
        const auto& currentTrace = current.second;

        const auto& it = inactivityProof.find(currentState);
        if (it != inactivityProof.end()) {
            Trace nextTrace(currentTrace.begin(), currentTrace.end());

            for (const auto& t : it->second) {
                Statement* stmt = t.first;
                const IntersectionState& nextState = t.second;
                if (stmt)
                    nextTrace.push_back(stmt);
                q.push(make_pair(nextState, nextTrace));

                if (stmt)
                    nextTrace.pop_back();
            }
        }
        else { // leaf node
            counterExamples.insert(currentTrace);
        }
    }

    return counterExamples;
}

set<Trace> LoopingTreeAutomataVerifier::proofCheckWithAntiChains(NFA *cfg, DFA *proof) {

    // maps (q_p, B, q_pi) to a set of set of statements
    map<T, set<set<Statement*>>> X;

    // the possible alphabet of sleep set
    set<Statement*> possibleStatementsInSleepSet;
    const auto& independenceRelation = _program->getIndependenceRelation();

    for (const auto& it: independenceRelation) {
        possibleStatementsInSleepSet.insert(it.second.begin(), it.second.end());
    }

    // orderings
    vector<const unordered_set<Statement*>*> R;
    R.reserve(_program->getTotalNumThreads());
    for (int i = 1; i < _program->getTotalNumThreads(); i ++) {
        R.push_back(&_program->getStatementsByThread(i));
    }

    //initial inactive states
    for (const auto& s1: cfg->getAcceptStates()) {
        for (const auto& s2: proof->getStates()) {
            if (!proof->isAcceptState(s2)) {
                T t(s1, false, s2);
                X[t].insert(possibleStatementsInSleepSet);
            }
        }
    }

    //inactivity proof
    map<T, map<set<Statement*>, map<Statement*, IntersectionState>>> inactivityProof;

    bool fixed = false;

    while (!fixed) {
        int numStates = 0;
        for (const auto&x : X) {
            numStates += x.second.size();
        }

        fixed = true;
        
        for (const auto& s1: cfg->getStates()) {
            if (!cfg->hasTransitionFrom(s1)) {
                continue;
            }

            for (const auto& s2: proof->getStates()) {

                T t(s1, false, s2);

                //this state already in the inactive states, skip
                if (X.find(t) != X.end()) {
                    continue;
                }

                set<set<Statement*>> X_meet;
                X_meet.insert(possibleStatementsInSleepSet);

                // forall linear orderings
                sort(R.begin(), R.end());
                do {
                    set<set<Statement*>> X_join;

                    for (const auto& transit: cfg->getTransitions(s1)) {
                        Statement* statement = transit.first;
                        const auto& targetStates = transit.second;

                        for (const auto& targetState: targetStates) {
                            uint32_t s1_next = targetState;
                            uint32_t s2_next;

                            if (statement == nullptr) {
                                s2_next = s2;
                            }
                            else {
                                s2_next = proof->getTargetState(s2, statement);
                            }

                            T next_t(s1_next, false, s2_next);

                            // transit into an inactive state
                            if (X.find(next_t) != X.end()) {
                                for (const auto& S : X[next_t]) {

                                    if (statement == nullptr) {
                                        set<set<Statement*>> temp;
                                        antiChainJoin(X_join, {S}, temp);
                                        X_join.clear();
                                        X_join.insert(temp.begin(), temp.end());

                                        LTAState nextLTAState(s1_next, false, S);
                                        IntersectionState nextIntersectionState(nextLTAState, s2_next);

                                        addToInactivityProof(inactivityProof, t, S, statement, nextIntersectionState);
                                    }
                                    else {
                                        set<Statement*> R_a;
                                        for (const auto & set : R) {
                                            if (set->find(statement) != set->end()) {
                                                break;
                                            }
                                            else {
                                                R_a.insert(set->begin(), set->end());
                                            }
                                        }

                                        const auto& D_a_u = _program->getDependentStatements(statement);
                                        set<Statement*> D_a(D_a_u.begin(), D_a_u.end());
                                        set<Statement*> R_a_minus_D_a;
                                        setDifference(R_a, D_a, R_a_minus_D_a);
                                        bool include = setInclusion(R_a_minus_D_a, S);

                                        if (include) {
                                            set<Statement*> S_union_D_a;
                                            setUnion(S, D_a, S_union_D_a);
                                            set<Statement*> S_union_D_a_minus_a;
                                            setDifference(S_union_D_a, {statement}, S_union_D_a_minus_a);
                                            set<set<Statement*>> temp;
                                            antiChainJoin(X_join, {S_union_D_a_minus_a}, temp);
                                            X_join.clear();
                                            X_join.insert(temp.begin(), temp.end());
                                            
                                            s2_next = proof->getTargetState(s2, statement);

                                            LTAState nextLTAState(s1_next, false, S);
                                            IntersectionState nextIntersectionState(nextLTAState, s2_next);
                                            addToInactivityProof(inactivityProof, t, S_union_D_a_minus_a, statement, nextIntersectionState);
                                        }
                                    }
                                }
                            }
                        }
                    }

                    set<set<Statement*>> temp;
                    antiChainMeet(X_meet, X_join, temp);
                    X_meet.clear();
                    X_meet.insert(temp.begin(), temp.end());

                }
                while (next_permutation(R.begin(), R.end()));

                // check if current state is inactive
                if (!X_meet.empty()) { // if inactive

                    // check start state
                    if (s1 == cfg->getStartState() && s2 == proof->getStartState()) {
                        T initState(s1,false,s2);
                        return getCounterExamples(inactivityProof, initState);
                    }

                    // update inactive states
                    if (X.find(t) == X.end()) {
                        X[t] = X_meet;
                        fixed = false;
                    }
                    else {
                        if (X[t] != X_meet) {
                            X[t] = X_meet;
                            fixed = false;
                        }
                    }
                }
                else { // not inactive
                    const auto& it = inactivityProof.find(t);
                    if (it != inactivityProof.end()) {
                        inactivityProof.erase(t);
                    }
                }
            }
        }
    }



    return {};
}

void LoopingTreeAutomataVerifier::addToInactivityProof(map<T, map<set<Statement *>, map<Statement *, IntersectionState>>> &inactivityProof,
                                                   T &fromState, const set<Statement *> &S, Statement *statement,
                                                   IntersectionState &toState) {

    const auto& it = inactivityProof.find(fromState);
    if (it != inactivityProof.end()) {
        bool isSubsumedByAny = false;
        vector<set<Statement*>> toRemove;
        for (const auto& it1 : it->second) {
            const set<Statement*>& s = it1.first;
            if (setInclusion(S, s)) {
                isSubsumedByAny = true;
                break;
            }

            if (setInclusion(s, S)) {
                toRemove.push_back(s);
            }
        }

        if (!toRemove.empty()) {
            for (const auto& r : toRemove) {
                it->second.erase(r);
            }
        }

        if (!isSubsumedByAny) {
            inactivityProof[fromState][S][statement] = toState;
        }
    }
    else {
        inactivityProof[fromState][S][statement] = toState;
    }
}

set<Trace> LoopingTreeAutomataVerifier::getCounterExamples(
        const map<T, map<set<Statement *>, map<Statement *, IntersectionState>>> &inactivityProof, T &initialState) {

    logger.debug(inactivityProofToString(inactivityProof));

    set<Trace> counterExamples;
    queue<pair<IntersectionState, Trace>> q;

    const auto& it = inactivityProof.find(initialState);
    if (it == inactivityProof.end()) {
        assert(false && "Error!");
    }

    const set<Statement*>& s = it->second.begin()->first;

    LTAState ls(get<0>(initialState), false, s);
    IntersectionState is(ls, get<2>(initialState));

    q.push(make_pair(is, Trace({})));

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        const auto& currentState = current.first;
        const auto& currentTrace = current.second;
        
        T currentT(get<0>(currentState.first), false, currentState.second);
        
        const auto& currentS = get<2>(currentState.first);

        const auto& it1 = inactivityProof.find(currentT);
        if (it1 != inactivityProof.end()) {
            Trace nextTrace(currentTrace.begin(), currentTrace.end());
            
            bool existATransition = false;
            for (const auto& it2 : it1->second) {
                const auto& maxS = it2.first;
                if (setInclusion(currentS, maxS)) {
                    
                    existATransition = true;

                    for (const auto& t: it2.second) {
                        Statement* stmt = t.first;
                        const IntersectionState& nextState = t.second;
                        
                        if (stmt)
                            nextTrace.push_back(stmt);
                        
                        q.push(make_pair(nextState, nextTrace));
                        
                        if (stmt)
                            nextTrace.pop_back();
                    }
                }
            }       

            if (!existATransition) {
                counterExamples.insert(currentTrace);
            }
        }
        else { // leaf node
            counterExamples.insert(currentTrace);
        }
    }

    return counterExamples;
}

void LoopingTreeAutomataVerifier::alphabetPowerSetGenerationHelper(unordered_set<Statement *>::const_iterator it, const Alphabet &alphabet,
                                                           set<Statement *> tempSet, set<set<Statement*>> &powerSetSet) {
    powerSetSet.insert(tempSet);

    if (it == alphabet.end())
        return;

    for (auto localIt = it; localIt != alphabet.end(); localIt++) {
        tempSet.insert(*localIt);
        alphabetPowerSetGenerationHelper(++it, alphabet, tempSet, powerSetSet);
        const auto& it1 = tempSet.find(*localIt);
        tempSet.erase(it1);
    }
}

void LoopingTreeAutomataVerifier::alphabetPowerSetGenerationHelper(set<Statement *>::const_iterator it,
                                                               const set<Statement *> &alphabet,
                                                               set<Statement *> tempSet,
                                                               set<set<Statement *>> &powerSetSet) {
    powerSetSet.insert(tempSet);

    if (it == alphabet.end())
        return;

    for (auto localIt = it; localIt != alphabet.end(); localIt++) {
        tempSet.insert(*localIt);
        alphabetPowerSetGenerationHelper(++it, alphabet, tempSet, powerSetSet);
        const auto& it1 = tempSet.find(*localIt);
        tempSet.erase(it1);
    }
}







