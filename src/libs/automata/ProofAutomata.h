#pragma once

#include "NFA.h"
#include <unordered_map>
#include "SMTSolverBase.h"

using namespace std;

namespace weaver {
    class ProofAutomata : public NFA {
    public:
        ProofAutomata(const Alphabet &alphabet, VariableTable* table);

        ~ProofAutomata() { delete _solver; }

        /**
         * Adding assertions to this proof automata
         * @param interpolants
         * @param alphabet
         */
        void extend(const Interpolants &interpolants, const Alphabet &alphabet);
        string getProof();

    private:

        // map the assertions to states
        unordered_map<string , uint32_t> _assertionMap;
        SMTSolverBase* _solver;
    };
}




