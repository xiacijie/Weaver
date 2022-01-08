#include "ParallelProgramVerifier.h"
#include "../theoremprover/SMTInterpol.h"
#include "../theoremprover/MathSAT.h"
#include "../theoremprover/Yices.h"
#include "InterpolantAutomataBuilder.h"
#include "../automata/ProofAutomata.h"
#include <chrono>

#define COUNTER_EXAMPLE_SELECTION 0

using namespace weaver;
using namespace std;
using namespace util;


using clo = std::chrono::system_clock;
using sec = std::chrono::duration<double>;

bool ParallelProgramVerifier::verify() {
    cout << "Start verifying... " << endl;
    bool correct = false;
    Program* program = _program;

    // eliminate those epsilon transitions
    NFA* cfg = program->getCFG().NFAEpsilonToNFA(program->getAlphabet());

    cout << cfg->toString() << endl;

    // start with an empty proof automata
    auto* proof = new ProofAutomata(program, program->getYices());

    int round = 1;
    double proofCheckingTime = 0;

    while (true) {
        cout << "=========== Round: " << round++ << " ==============" << endl;
        cout << "Proof Size : " << proof->getNumStates() << endl;

        cout << "Convert Proof to DFA..." << endl;
        DFA* DProof = proof->NFAToDFA(program->getAlphabet());

        cout << "Getting Error Trace..." << endl;
        const auto before = clo::now();
        auto errorTraceSet = proofCheckWithAntiChains(cfg, DProof);
        const sec duration = clo::now() - before;

        cout << duration.count() << endl;

        proofCheckingTime += duration.count();

        delete DProof;

        if (!errorTraceSet.empty()) {
            cout << "Get a set of error traces..." << endl;
            if (COUNTER_EXAMPLE_SELECTION == 0) {
                const auto& trace = *errorTraceSet.begin();
                for (const auto& t: trace) {
                    cout << t->toString() << " || ";
                }
                cout << endl;

                // This trace must be non-empty
                Interpolants interpols = _program->getMathSAT()->generateInterpols(trace);
                if (interpols.empty()) {
                    cerr << "This Program is Incorrect!" << endl;
                    cerr << "A valid counterexample trace:" << endl << endl;
                    for (auto t : trace) {
                        cerr << t->toString() << endl;
                    }

                    break;
                }

                cout << "Interpolants: ";
                for (const auto& intpl: interpols) {
                    cout << intpl << endl;
                }
                cout << endl;

                proof->extend(interpols, program->getAlphabet());
            }
            else  {
                bool anyIncorrectTrace = false;
                int i = 1;
                for (const auto& trace : errorTraceSet) {
                    cout << i++ << ". ";
                    for (const auto& t: trace) {
                        cout << t->toString() << " || ";
                    }
                    cout << endl;

                    // This trace must be non-empty
                    Interpolants interpols = _program->getMathSAT()->generateInterpols(trace);
                    if (interpols.empty()) {
                        cerr << "This Program is Incorrect!" << endl;
                        cerr << "A valid counterexample trace:" << endl << endl;
                        for (auto t : trace) {
                            cerr << t->toString() << endl;
                        }

                        anyIncorrectTrace = true;
                        break;
                    }

                    cout << "Interpolants: ";
                    for (const auto& intpl: interpols) {
                        cout << intpl << endl;
                    }
                    cout << endl;

                    proof->extend(interpols, program->getAlphabet());
                }

                if (anyIncorrectTrace) {
                    break;
                }
            }


        }
        else {
            cout << "Proof Checking Time: " << proofCheckingTime << "s" << endl;
            cout << "***********************************************" << endl;
            cout << "**   End: The program is verified correct!   **" << endl;
            cout << "***********************************************" << endl;

            correct = true;
            break;
        }
    }

    delete proof;
    delete cfg;
    return correct;
}


set<Trace> ParallelProgramVerifier::proofCheck(NFA* cfg, DFA* proof) {

    map<IntersectionState, map<Statement*, IntersectionState>> inactivityProof;

    set<IntersectionState> inactiveStates;
    bool inactiveStatesChanged = true;

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
    while (inactiveStatesChanged) {
        cout << inactiveStates.size() << endl;
        inactiveStatesChanged = false;
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
                                S_union_R_a.insert(sleepSet.begin(), sleepSet.end());
                                S_union_R_a.insert(R_a.begin(), R_a.end());

                                set<Statement*> S_union_R_a_minus_D_a;
                                const auto& D_a = _program->getDependentStatements(stmt);

                                for (const auto& ss : S_union_R_a) {
                                    if (D_a.find(ss) == D_a.end()) {
                                        S_union_R_a_minus_D_a.insert(ss);
                                    }
                                }

                                nextSleepSet.insert(S_union_R_a_minus_D_a.begin(), S_union_R_a_minus_D_a.end());


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
                            cout << "Initial State is inactive" << endl;
                            LTAState initialLTAState(s1, false, {});
                            IntersectionState initialIntersectionState(initialLTAState, s2);

                            set<Trace> counterExamples = getCounterExamples(inactivityProof, initialIntersectionState);

                            return counterExamples;
                        }

                        inactiveStatesChanged = true;

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

set<Trace> ParallelProgramVerifier::getCounterExamples(
        const map <IntersectionState, map<Statement *, IntersectionState>> &inactivityProof,
        IntersectionState& initialState) {

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
                nextTrace.push_back(stmt);
                q.push(make_pair(nextState, nextTrace));
                nextTrace.pop_back();
            }
        }
        else { // leaf node
            counterExamples.insert(currentTrace);
        }
    }

    return counterExamples;
}

set<Trace> ParallelProgramVerifier::proofCheckWithAntiChains(NFA *cfg, DFA *proof) {

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
    map<T, map<set<Statement*>, map<Statement*, T>>> inactivityProof;

    bool fixed = false;

    while (!fixed) {
        int numStates = 0;
        for (const auto&x : X) {
            numStates += x.second.size();
        }

        cout << "States: " << numStates << endl;
        fixed = true;
        
        for (const auto& s1: cfg->getStates()) {
            if (!cfg->hasTransitionFrom(s1)) {
                continue;
            }

            for (const auto& s2: proof->getStates()) {

                T t(s1, false, s2);
                set<set<Statement*>> X_meet;
                X_meet.insert(possibleStatementsInSleepSet);

                sort(R.begin(), R.end());
                do {
                    set<set<Statement*>> X_join;

                    for (const auto& transit: cfg->getTransitions(s1)) {
                        Statement* statement = transit.first;
                        const auto& targetStates = transit.second;

                        for (const auto& targetState: targetStates) {
                            uint32_t s1_next;
                            uint32_t s2_next;

                            s1_next = targetState;
                            s2_next = proof->getTargetState(s2, statement);

                            T next_t(s1_next, false, s2_next);

                            // transit into an inactive state
                            if (X.find(next_t) != X.end()) {
                                for (const auto& S : X[next_t]) {
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

                                        addToInactivityProof(inactivityProof, t, S_union_D_a_minus_a, statement, next_t);
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
                // if inactive
                if (!X_meet.empty()) {

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

void ParallelProgramVerifier::addToInactivityProof(map<T, map<set<Statement *>, map<Statement *, T>>> &inactivityProof,
                                                   T &fromState, const set<Statement *> &S, Statement *statement,
                                                   T &toState) {
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

set<Trace> ParallelProgramVerifier::getCounterExamples(
        const map<T, map<set<Statement *>, map<Statement *, T>>> &inactivityProof, T &initialState) {

    set<Trace> counterExamples;
    queue<pair<T, Trace>> q;

    q.push(make_pair(initialState, Trace({})));

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        const auto& currentState = current.first;
        const auto& currentTrace = current.second;

        const auto& it = inactivityProof.find(currentState);
        if (it != inactivityProof.end()) {
            Trace nextTrace(currentTrace.begin(), currentTrace.end());

            for (const auto& tm : it->second) {
                for (const auto& t : tm.second) {
                    Statement* stmt = t.first;
                    const T& nextState = t.second;
                    nextTrace.push_back(stmt);
                    q.push(make_pair(nextState, nextTrace));
                    nextTrace.pop_back();
                }
            }
        }
        else { // leaf node
            counterExamples.insert(currentTrace);
        }
    }

    return counterExamples;
}

void ParallelProgramVerifier::alphabetPowerSetGenerationHelper(unordered_set<Statement *>::const_iterator it, const Alphabet &alphabet,
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

void ParallelProgramVerifier::alphabetPowerSetGenerationHelper(set<Statement *>::const_iterator it,
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







