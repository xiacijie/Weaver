#include "SequentialProgramVerifier.h"
#include "InterpolantAutomataBuilder.h"

#include <iostream>
#include <queue>
#include <algorithm>

using namespace weaver;
using namespace std;


void SequentialProgramVerifier::verify() {

    cout << "Start Verifying..." << endl;

    NFA* cfg = _cfg;

    DFA* proofAutomata = nullptr;

    int round = 0;

    while (true) {
        cout << "!*************  Verification Round: " << ++round << "****************!" << endl;

        cout << "1. Get error trace..." << endl;

        Trace errorTrace = getErrorTrace(cfg, proofAutomata);

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
        Interpolants interpolants = _prover->generateInterpols(errorTrace);

        if (interpolants.empty()) {
            cerr << "This Program is Incorrect!" << endl;
            cerr << "A valid counterexample trace:" << endl << endl;
            for (auto t : errorTrace) {
                cerr << t->toString() << endl;
            }
            abort();
        }

        cout << "3. Construct Interpolant Automata...  " << endl;
        NFA* interpolAutomata = InterpolantAutomataBuilder::build(errorTrace, interpolants, _prover, _program);
        DFA* determinsticInterpolAutomata = interpolAutomata->convertToDFA(_program->getAlphabet());
        delete interpolAutomata;

        cout << "4. Minimize interpolant automata..." << endl;
        determinsticInterpolAutomata->minimize(_program->getAlphabet());

        cout << "5. Construct Union..." << endl;
        if (!proofAutomata) {
            proofAutomata = determinsticInterpolAutomata;
        }
        else {
            DFA* lastProofAutomata  = proofAutomata;
            proofAutomata = proofAutomata->Union(determinsticInterpolAutomata, _program->getAlphabet());
            delete lastProofAutomata;
            delete determinsticInterpolAutomata;
        }

        cout << "Proof size: " << proofAutomata->getNumStates() << endl;

        proofAutomata->minimize(_program->getAlphabet());
    }

    cout << "Fine" << endl;

}

Trace SequentialProgramVerifier::getErrorTrace(NFA *cfg, DFA *proofAutomata) {

    if (proofAutomata == nullptr) {
        proofAutomata = new DFA();
        proofAutomata->addState(0);
        proofAutomata->setStartState(0);
        proofAutomata->complete(1, _program->getAlphabet());
    }

    assert(proofAutomata->isComplete(_program->getAlphabet()) && "Error");

    // verify the inclusion on the fly
    set<pair<uint32_t, uint32_t>> states;

    queue<pair<pair<uint32_t, uint32_t>, Trace>> workList;

    auto startState = make_pair(cfg->getStartState(), proofAutomata->getStartState());

    Trace empty;
    workList.push(make_pair(startState, empty));

    while (!workList.empty()) {
        auto currentState = workList.front().first;
        auto currentTrace = workList.front().second;
        workList.pop();

        states.insert(currentState);

        if (cfg->isAcceptState(currentState.first) && !proofAutomata->isAcceptState(currentState.second)) {
            return currentTrace;
        }

        if (_cfg->hasTransitionFrom(currentState.first)) {
            const auto &transitions = _cfg->getTransitions(currentState.first);
            for (const auto &it: transitions) {
                Statement *stmt = it.first;

                for (const auto& nextState1 : it.second) {
                    uint32_t nextState2;

                    if (stmt == nullptr) {
                        nextState2 = currentState.second;
                    } else {
                        nextState2 = proofAutomata->getTargetState(currentState.second, stmt);
                    }

                    auto nextState = make_pair(nextState1, nextState2);

                    if (states.find(nextState) == states.end()) {
                        Trace t(currentTrace.begin(), currentTrace.end());

                        if (stmt)
                            t.push_back(stmt);

                        workList.push(make_pair(nextState, t));
                    }
                }

            }
        }

    }
    return empty;
}
