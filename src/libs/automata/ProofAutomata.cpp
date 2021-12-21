#include "ProofAutomata.h"
#include "../verifier/SMTInterpol.h"

using namespace weaver;

ProofAutomata::ProofAutomata(Program *program) {
    _prover = new SMTInterpol(program);
    addState(0);
    _assertionMap["false"] = 0;
    setAcceptState(0);

    addState(1);
    _assertionMap["true"] = 1;
    setStartState(1);

    for (const auto& it1 : _assertionMap) {
        const string& assertion1 = it1.first;
        uint32_t state1 = it1.second;
        for (const auto& it2 : _assertionMap) {
            const string& assertion2 = it2.first;
            uint32_t state2 = it2.second;
            for (const auto& stmt : program->getAlphabet()) {

                bool valid = _prover->checkHoareTripe(assertion1, stmt, assertion2);
                if (valid) {
                    addTransition(state1, stmt, state2);
                }
            }
        }
    }
}

void ProofAutomata::extend(const Interpolants &interpolants, const Alphabet &alphabet) {
    for (const auto& interpol: interpolants) {
        // check if this is a new assertion that is not in the proof
        cout << interpol << endl;
        if (_assertionMap.find(interpol) == _assertionMap.end()) {
            uint32_t newState = getNumStates();
            addState(newState);

            _assertionMap[interpol] = newState;

            for (const auto& it : _assertionMap) {
                const string& assertion = it.first;
                uint32_t state = it.second;
                for (const auto& stmt : alphabet) {

                    if (assertion != interpol) {
                        bool valid =  _prover->checkHoareTripe(interpol, stmt, assertion);
                        if (valid) {
                            addTransition(newState,stmt, state);
                        }

                        valid = _prover->checkHoareTripe(assertion, stmt, interpol);

                        if (valid) {
                            addTransition(state, stmt, newState);
                        }
                    }
                    else {
                        bool valid = _prover->checkHoareTripe(interpol, stmt, interpol);
                        if (valid) {
                            addTransition(newState, stmt, newState);
                        }
                    }

                }

            }
        }
    }
}