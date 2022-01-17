#pragma once
#include "Program.h"
#include "NFA.h"
#include "SMTInterpol.h"
#include "VerifierBase.h"
#include <vector>

using namespace std;
namespace weaver {

    /**
     * The program verifier using finite automata
     */
    class FiniteAutomataVerifier : public VerifierBase {
    public:
        FiniteAutomataVerifier(Program* program) :
            VerifierBase(program)
            {}

        bool verify() override;
    private:
        Trace proofCheck(NFA* cfg, DFA* proof);
        void proofCheckHelper(NFA* cfg, DFA* proof, 
                                set<pair<uint32_t, uint32_t>>& states,
                                pair<uint32_t, uint32_t> currentState, 
                                Trace currentTrace,
                                bool& errorTraceFound,
                                Trace& errorTrace);
    };
}



