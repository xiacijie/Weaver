#include "ParallelProgramVerifier.h"
#include "SMTInterpol.h"
#include "InterpolantAutomataBuilder.h"

using namespace weaver;
using namespace std;

bool ParallelProgramVerifier::verify() {
    bool correct = false;

    Program* program = _program;
    NFA* cfg = &program->getCFG();

    // start with an empty proof
    DFA* proof = new DFA();
    proof->addState(0);
    proof->setStartState(0);
    proof->complete(1, program->getAlphabet());

    SMTInterpol* prover = new SMTInterpol(program);

    while (true) {
        auto errorTraceSet = getErrorTraces(cfg, proof);
        if (errorTraceSet.size() == 1) {
            const Trace& errorTrace = *errorTraceSet.begin();
            // This trace must be non-empty
            Interpolants interpols = prover->generateInterpols(errorTrace);

            if (interpols.empty()) {
                cerr << "This Program is Incorrect!" << endl;
                cerr << "A valid counterexample trace:" << endl << endl;
                for (auto t : errorTrace) {
                    cerr << t->toString() << endl;
                }

                break;
            }

            NFA* interpolAutomata = InterpolantAutomataBuilder::build(errorTrace, interpols, prover, program);
            DFA* DInterpolAutomata = interpolAutomata->convertToDFA(program->getAlphabet());
            delete interpolAutomata;

            DInterpolAutomata->minimize(program->getAlphabet());

            DFA* lastProofAutomata = proof;

            proof = proof->Union(DInterpolAutomata, program->getAlphabet());
            delete DInterpolAutomata;
            delete lastProofAutomata;
        }
        else {
            cout << "***********************************************" << endl;
            cout << "**   End: The program is verified correct!   **" << endl;
            cout << "***********************************************" << endl;

            correct = true;
            break;
        }
    }

    delete prover;
    return correct;
}


set<Trace> ParallelProgramVerifier::getErrorTraces(NFA* cfg, DFA* proof) {
    set<Trace> result;

    // set the reduction ordering R
    // The ordering uses the partition optimization
    // Note that the main thread is dependent with any other threads
    vector<const unordered_set<Statement*>*> R;

    R.reserve(_program->getTotalNumThreads());
    for (int i = 1; i < _program->getTotalNumThreads(); i ++) {
        R.push_back(&_program->getStatementsByThread(i));
    }

    sort(R.begin(), R.end());

    // construct the LTA and checking the emptiness on the fly
    // we need to find if an inactive state is reachable from a start state
    // Therefore, searching in BFS

    // the start state of LTA
    tuple<uint32_t, bool, set<Statement*>> LTAStartState(cfg->getStartState(), false, {});

    // (LTA States x DFA_proof States)
    set<pair<tuple<uint32_t, bool, set<Statement*>>, uint32_t>> intersectionStates;

    queue<pair<pair<tuple<uint32_t, bool, set<Statement*>>, uint32_t>, Trace>> workList;

    Trace empty;
    workList.push(make_pair(make_pair(LTAStartState, proof->getStartState()), empty));

    while (!workList.empty()) {
        auto currentIntersectionState = workList.front().first;
        Trace currentTrace = workList.front().second;

        workList.pop();

        do {

            auto currentLTAState = currentIntersectionState.first;
            auto currentProofDFAState = currentIntersectionState.second;

            const auto& sleepSet = get<set<Statement*>>(currentLTAState);

            auto q = get<uint32_t>(currentLTAState);
            auto i = get<bool>(currentLTAState);

            intersectionStates.insert(currentIntersectionState);

            // verify the next available transitions
            bool B_LTA = cfg->isAcceptState(get<uint32_t>(currentLTAState)) && !i;

            if (!proof->isAcceptState(currentProofDFAState)) {
                if (B_LTA == true) { // we found an inactive state
                    cerr << "Inactive State Found!" << endl;

                    for (const auto& s: currentTrace) {
                        cerr << s->toString() << endl;
                    }

                    result.insert(currentTrace);
                    return result;
                }
            }

            if (cfg->hasTransitionFrom(q)) {
                for (const auto & t: cfg->getTransitions(q)) {
                    Statement* statement = t.first;
                    const unordered_set<uint32_t>& targetStates = t.second;

                    for (const auto& s: targetStates) {
                        uint32_t next_q;
                        bool next_i;
                        set<Statement*> nextSleepSet;

                        uint32_t nextProofDFAState;

                        if (statement == nullptr) { // epsilon transition
                            next_q = s;
                            next_i = i;
                            nextSleepSet.insert(sleepSet.begin(), sleepSet.end());
                            nextProofDFAState = currentProofDFAState;
                        }
                        else {
                            next_q = s;
                            next_i = i || (sleepSet.find(statement) != sleepSet.end());

                            set<Statement*> R_a;
                            for (const auto & set : R) {
                                if (set->find(statement) != set->end()) {
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
                            const auto& D_a = _program->getDependentStatements(statement);

                            for (const auto& ss : S_union_R_a) {
                                if (D_a.find(ss) == D_a.end()) {
                                    S_union_R_a_minus_D_a.insert(ss);
                                }
                            }

                            nextSleepSet.insert(S_union_R_a_minus_D_a.begin(), S_union_R_a_minus_D_a.end());
                            nextProofDFAState = proof->getTargetState(currentProofDFAState, statement);

                        }

                        tuple<uint32_t, bool, set<Statement*>> nextLTAState(next_q, next_i, nextSleepSet);

                        auto nextIntersectionState = make_pair(nextLTAState, nextProofDFAState);

                        if (intersectionStates.find(nextIntersectionState) == intersectionStates.end()) {
                            Trace nextTrace(currentTrace.begin(), currentTrace.end());

                            if (statement) {
                                nextTrace.push_back(statement);
                            }

                            workList.push(make_pair(make_pair(nextLTAState, nextProofDFAState), nextTrace));
                        }
                    }
                }
            }
        } while (next_permutation(R.begin(), R.end()));
    }

    return result;
}