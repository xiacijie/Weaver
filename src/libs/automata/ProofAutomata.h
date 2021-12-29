#pragma once

#include "NFA.h"
#include <unordered_map>
#include "../verifier/TheoremProverBase.h"

using namespace std;

namespace weaver {
    class ProofAutomata : public NFA {
    public:
        ProofAutomata(Program* program, TheoremProverBase* prover);

        /**
         * Adding assertions to this proof automata
         * @param interpolants
         * @param alphabet
         */
        void extend(const Interpolants &interpolants, const Alphabet &alphabet);

    private:

        // map the assertions to states
        unordered_map<string , uint32_t> _assertionMap;
        TheoremProverBase* _prover;
    };
}




