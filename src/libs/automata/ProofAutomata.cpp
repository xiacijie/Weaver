#include "ProofAutomata.h"
#include "SMTInterpol.h"
#include "Program.h"
#include "Config.h"

using namespace weaver;

extern Config config;

string ProofAutomata::getProof() {
    stringstream ss;
    for (const auto& it : _assertionMap) {
        ss << it.first << endl;
    }

    return ss.str();
}

ProofAutomata::ProofAutomata(const Alphabet &alphabet, VariableTable* table) {

    if (config.hoareTripleSMTSolver == SMTSolverType::mathsat) {
        _solver = new MathSAT(table);
    }
    else if (config.hoareTripleSMTSolver == SMTSolverType::yices) {
        _solver = new Yices(table);
    }
    else {
        _solver = new SMTInterpol(table);
    }

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
            for (const auto& stmt : alphabet) {

                bool valid = _solver->checkHoareTripe(assertion1, stmt, assertion2);
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
        if (_assertionMap.find(interpol) == _assertionMap.end()) {
            uint32_t newState = getNumStates();
            addState(newState);

            _assertionMap[interpol] = newState;

            for (const auto& it : _assertionMap) {
                const string& assertion = it.first;
                uint32_t state = it.second;
                for (const auto& stmt : alphabet) {

                    if (assertion != interpol) {
                        bool valid =  _solver->checkHoareTripe(interpol, stmt, assertion);
                        if (valid) {
                            addTransition(newState,stmt, state);
                        }

                        valid = _solver->checkHoareTripe(assertion, stmt, interpol);

                        if (valid) {
                            addTransition(state, stmt, newState);
                        }
                    }
                    else {
                        bool valid = _solver->checkHoareTripe(interpol, stmt, interpol);
                        if (valid) {
                            addTransition(newState, stmt, newState);
                        }
                    }

                }

            }
        }
    }
}