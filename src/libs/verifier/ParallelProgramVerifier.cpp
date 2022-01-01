#include "ParallelProgramVerifier.h"
#include "../theoremprover/SMTInterpol.h"
#include "../theoremprover/MathSAT.h"
#include "../theoremprover/Yices.h"
#include "InterpolantAutomataBuilder.h"
#include "../automata/ProofAutomata.h"

#define COUNTER_EXAMPLE_SELECTION 0

using namespace weaver;
using namespace std;

bool ParallelProgramVerifier::verify() {
    cout << "Start verifying... " << endl;
    bool correct = false;
    Program* program = _program;
    NFA* cfg = &program->getCFG();

    // start with an empty proof automata
    auto* proof = new ProofAutomata(program, _program->getYices());

    int round = 1;
    while (true) {
        cout << "=========== Round: " << round++ << " ==============" << endl;
        cout << "Proof Size : " << proof->getNumStates() << endl;

        cout << "Convert Proof to DFA..." << endl;
        DFA* DProof = proof->convertToDFA(program->getAlphabet());
        cout << "Minimizing DProof..." << endl;
        DProof->minimize(program->getAlphabet());

        cout << "Getting Error Trace..." << endl;
        auto errorTraceSet = proofCheck(cfg, DProof);
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
            cout << "***********************************************" << endl;
            cout << "**   End: The program is verified correct!   **" << endl;
            cout << "***********************************************" << endl;

            correct = true;
            break;
        }
    }

    delete proof;
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
        map <IntersectionState, map<Statement *, IntersectionState>> &inactivityProof,
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

                if (stmt != nullptr)
                    nextTrace.push_back(stmt);

                q.push(make_pair(nextState, nextTrace));

                if (stmt != nullptr)
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
    typedef tuple<uint32_t, bool, uint32_t> T;

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

    for (const auto& s1: cfg->getStates()) {
        if (!cfg->hasTransitionFrom(s1)) {
            continue;
        }

        for (const auto& s2: proof->getStates()) {

            set<set<Statement*>> X_meet;
            X_meet.insert(possibleStatementsInSleepSet);

            sort(R.begin(), R.end());
            do {
                set<set<Statement*>> X_join;
            }
            while (next_permutation(R.begin(), R.end()));
        }
    }

    return {};
}

//
//set<set<Statement*>> ParallelProgramVerifier::X(uint32_t q_cfg, uint32_t q_pi) {
//
//    return {};
//}
//
//set<set<Statement*>> ParallelProgramVerifier::FMax(function<set<set<Statement *>>(uint32_t, uint32_t)> F,
//                                                   uint32_t q_cfg, uint32_t q_pi, NFA* cfg, DFA* proof) {
//
//    if (cfg->isAcceptState(q_cfg) && !proof->isAcceptState(q_pi)) {
//        set<set<Statement*>> result;
//        result.insert(_program->getAlphabet().begin(), _program->getAlphabet().end());
//        return result;
//    }
//
//    set<set<Statement*>> X_n;
//    X_n.insert(_program->getAlphabet().begin(), _program->getAlphabet().end());
//
//    vector<const unordered_set<Statement*>*> R;
//    R.reserve(_program->getTotalNumThreads());
//    for (int i = 1; i < _program->getTotalNumThreads(); i ++) {
//        R.push_back(&_program->getStatementsByThread(i));
//    }
//
//    sort(R.begin(), R.end());
//    do {
//        set<set<Statement*>> X_u;
//
//        if (cfg->hasTransitionFrom(q_cfg)) {
//            for (const auto& t : cfg->getTransitions(q_cfg)) {
//                Statement* stmt = t.first;
//                const auto& targetStates = t.second;
//
//                for (const auto& targetState: targetStates) {
//
//                }
//            }
//        }
//    }
//    while (next_permutation(R.begin(), R.end()));
//
//}
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