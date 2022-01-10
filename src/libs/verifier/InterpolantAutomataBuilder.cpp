#include "InterpolantAutomataBuilder.h"
#include "Program.h"
using namespace weaver;
using namespace std;

NFA* InterpolantAutomataBuilder::build(const Trace &errorTrace, const Interpolants &interpols, TheoremProverBase *prover, Program* program) {
    NFA* automata = new NFA();
    automata->addState(0);
    automata->addState(1);

    automata->setAcceptState(0);
    automata->setStartState(1);

    uint32_t currentState = automata->getStartState();

    for (int i = 0 ; i < errorTrace.size() - 1; i++ ) {
        Statement* stmt = errorTrace[i];

        uint32_t newState = automata->getNumStates();
        automata->addState(newState);

        automata->addTransition(currentState, stmt, newState);
        currentState = newState;
    }

    automata->addTransition(currentState, errorTrace[errorTrace.size()-1], *automata->getAcceptStates().begin());

    /** set back-edges **/
    for (int i = 1; i < interpols.size() - 2; i++) {
        for (int j = i+1; j < interpols.size() - 1; j++) {
            if (prover->entails(interpols[j], interpols[i])) {
                uint32_t frontState = i + 1;
                uint32_t backState = j + 1;
                automata->addTransition(backState, nullptr, frontState);
            }
        }
    }

    return automata;
}