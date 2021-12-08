#pragma once
#include "../program/Program.h"
#include "TheoremProverBase.h"

namespace weaver {
    class ParallelProgramVerifier {
    public:
        explicit ParallelProgramVerifier(Program* program):
            _program(program)
        {}

        /**
         *
         * @return true if the program is correct.
         * false if the program is incorrect
         */
        bool verify();

    private:
        set<Trace> getErrorTraces(NFA* cfg, DFA* proof);
        Program* _program;
    };
}