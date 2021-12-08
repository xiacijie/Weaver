#pragma once
#include "../program/Program.h"
#include "../automata/NFA.h"

#include "SMTInterpol.h"
#include <vector>

using namespace std;
namespace weaver {

    /**
     * The correctness verifier for the sequential programs
     */
    class SequentialProgramVerifier {
    public:
        SequentialProgramVerifier(Program* program) :
            _program(program),
            _cfg(&program->getCFG()),
            _prover(new SMTInterpol(program))
            {}

        ~SequentialProgramVerifier() { delete _prover; }
        void verify();
    private:
        Trace getErrorTrace(NFA* cfg, DFA* proofAutomata);

        Program* _program;
        NFA* _cfg;
        SMTInterpol* _prover;

    };
}



