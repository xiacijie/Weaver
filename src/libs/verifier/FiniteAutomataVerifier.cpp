#include "FiniteAutomataVerifier.h"
#include "ProofAutomata.h"
#include "Timer.h"

#include <iostream>
#include <queue>
#include <algorithm>

using namespace weaver;
using namespace std;


void FiniteAutomataVerifier::verify() {

    cout << "Start Verifying..." << endl;

    NFA* cfg = &_program->getCFG();

    ProofAutomata proof(_program, _program->getYices());

    int round = 0;

    Timer t1;
    Timer t2;
    double proofCheckTime = 0;
    double deterTime = 0;
    double extendTime = 0;

    while (true) {
        ++round;
        // cout << "!*************  Verification Round: " << ++round << "****************!" << endl;
        // cout << proof.getNumStates() << endl;
        // cout << proof.getProof() << endl;
        // cout << "1. Get error trace..." << endl;

        t2.start();
        DFA* Dproof = proof.NFAToDFA(_program->getAlphabet());
        cout << "DProof: " << Dproof->getNumStates() << endl;
        deterTime += t2.stop();

        t1.start();
        Trace errorTrace = proofCheck(cfg, Dproof);
        proofCheckTime += t1.stop();

        delete Dproof;

        if (errorTrace.empty()) {
            cout << "Size of CFG: " << cfg->getNumStates() << endl;
            cout << "Deter Time: " << deterTime << endl;
            cout << "Extend Time: " << extendTime << endl;
            cout << "Total proof checking time: " << proofCheckTime << endl;
            cout << "Total proof construction time: " << extendTime + deterTime << endl;
            cout << "Number of refinement rounds: " << round << endl;
            cout << "Size of proof: " << proof.getNumStates() << endl;

            cout << proof.getNumStates() << endl;
            cout << proof.getProof() << endl;
            cout << "***********************************************" << endl;
            cout << "**   End: The program is verified correct!   **" << endl;
            cout << "***********************************************" << endl;
            break;
        }

        // cout << "Error Trace:" << endl;
        // for (auto t : errorTrace) {
        //     cout << t->toString() << endl;
        // }

        // cout << "2. Do Craig Interpolantion ..." << endl;
        Interpolants interpolants = _program->getMathSAT()->generateInterpols(errorTrace);

        if (interpolants.empty()) {
            cerr << "This Program is Incorrect!" << endl;
            cerr << "A valid counterexample trace:" << endl << endl;
            for (auto t : errorTrace) {
                cerr << t->toString() << endl;
            }
            abort();
        }

        // cout << "3. Construct Proof Automata...  " << endl;
        t2.start();
        proof.extend(interpolants, _program->getAlphabet());
        extendTime += t2.stop();


        // cout << "Proof size: " << proof.getNumStates() << endl;
    }

    // cout << "Fine" << endl;

}

Trace FiniteAutomataVerifier::proofCheck(NFA* cfg, DFA* proof) {
    // verify the inclusion on the fly
    set<pair<uint32_t, uint32_t>> states;

    auto startState = make_pair(cfg->getStartState(), proof->getStartState());
    Trace errorTrace;
    bool errorTraceFound = false;

    proofCheckHelper(cfg, proof, states, startState, {}, errorTraceFound, errorTrace);

    return errorTrace;
}

void FiniteAutomataVerifier::proofCheckHelper(NFA* cfg, DFA* proof,
                                                set<pair<uint32_t, uint32_t>>& states,
                                                pair<uint32_t, uint32_t> currentState,
                                                Trace currentTrace,
                                                bool& errorTraceFound,
                                                Trace& errorTrace) {
    if (errorTraceFound == true)
        return;

    states.insert(currentState);

    if (cfg->isAcceptState(currentState.first) && !proof->isAcceptState(currentState.second)) {
        errorTraceFound = true;
        errorTrace.insert(errorTrace.end(), currentTrace.begin(), currentTrace.end());
        return;
    }

    if (cfg->hasTransitionFrom(currentState.first)) {
        for (const auto& t : cfg->getTransitions(currentState.first)) {
            Statement* stmt = t.first;

            for (const auto& nextState1 : t.second) {
                uint32_t nextState2;

                if (stmt == nullptr) {
                    nextState2 = currentState.second;
                }
                else {
                    nextState2 = proof->getTargetState(currentState.second, stmt);
                }

                auto nextState = make_pair(nextState1, nextState2);

                if (states.find(nextState) == states.end()) {
                    if (stmt)
                        currentTrace.push_back(stmt);

                    proofCheckHelper(cfg, proof, states, nextState, currentTrace, errorTraceFound, errorTrace);
                    
                    if (stmt)
                        currentTrace.pop_back();
                }
            }
        }
    }

}
