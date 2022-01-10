#include "SequentialProgramVerifier.h"
#include "InterpolantAutomataBuilder.h"
#include "ProofAutomata.h"

#include <iostream>
#include <queue>
#include <algorithm>

using namespace weaver;
using namespace std;


void SequentialProgramVerifier::verify() {

    cout << "Start Verifying..." << endl;

    NFA* cfg = _program->getCFG().NFAEpsilonToNFA(_program->getAlphabet());

    ProofAutomata proof(_program, _program->getYices());

    int round = 0;

    while (true) {
        cout << "!*************  Verification Round: " << ++round << "****************!" << endl;

        cout << "1. Get error trace..." << endl;

        DFA* Dproof = proof.NFAToDFA(_program->getAlphabet());
        Trace errorTrace = proofCheck(cfg, Dproof);
        delete Dproof;

        if (errorTrace.empty()) {
            cout << "***********************************************" << endl;
            cout << "**   End: The program is verified correct!   **" << endl;
            cout << "***********************************************" << endl;
            break;
        }

        cout << "Error Trace:" << endl;
        for (auto t : errorTrace) {
            cout << t->toString() << endl;
        }

        cout << "2. Do Craig Interpolantion ..." << endl;
        Interpolants interpolants = _program->getMathSAT()->generateInterpols(errorTrace);

        if (interpolants.empty()) {
            cerr << "This Program is Incorrect!" << endl;
            cerr << "A valid counterexample trace:" << endl << endl;
            for (auto t : errorTrace) {
                cerr << t->toString() << endl;
            }
            abort();
        }

        cout << "3. Construct Proof Automata...  " << endl;
        proof.extend(interpolants, _program->getAlphabet());


        cout << "Proof size: " << proof.getNumStates() << endl;
    }

    cout << "Fine" << endl;

}

Trace SequentialProgramVerifier::proofCheck(NFA* cfg, DFA* proof) {
    // verify the inclusion on the fly
    set<pair<uint32_t, uint32_t>> states;

    auto startState = make_pair(cfg->getStartState(), proof->getStartState());
    Trace errorTrace;
    bool errorTraceFound = false;

    proofCheckHelper(cfg, proof, states, startState, {}, errorTraceFound, errorTrace);

    return errorTrace;
}

void SequentialProgramVerifier::proofCheckHelper(NFA* cfg, DFA* proof,
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
            if (stmt == nullptr) {
                cout << "oops" << endl;
            }
            for (const auto& nextState1 : t.second) {
                uint32_t nextState2 = proof->getTargetState(currentState.second, stmt);
                auto nextState = make_pair(nextState1, nextState2);

                if (states.find(nextState) == states.end()) {
                    currentTrace.push_back(stmt);
                    proofCheckHelper(cfg, proof, states, nextState, currentTrace, errorTraceFound, errorTrace);
                    currentTrace.pop_back();
                }
            }
        }
    }

}
