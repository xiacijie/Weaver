#pragma once
#include "Program.h"
#include "NFA.h"
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
            _program(program)
            {}

        void verify();
    private:
        Trace proofCheck(NFA* cfg, DFA* proof);
        void proofCheckHelper(NFA* cfg, DFA* proof, 
                                set<pair<uint32_t, uint32_t>>& states,
                                pair<uint32_t, uint32_t> currentState, 
                                Trace currentTrace,
                                bool& errorTraceFound,
                                Trace& errorTrace);

        Program* _program;
    };
}



