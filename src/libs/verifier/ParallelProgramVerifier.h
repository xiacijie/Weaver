#pragma once
#include "../program/Program.h"
#include "TheoremProverBase.h"
#include <functional>

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
        typedef tuple<uint32_t, bool, set<Statement*>> LTAState;
        typedef pair<LTAState, uint32_t> IntersectionState;

        set<Trace> proofCheck(NFA* cfg, DFA* proof);

        set<Trace> proofCheckWithAntiChains(NFA* cfg, DFA* proof);
        set<set<Statement*>> X(uint32_t q_cfg, uint32_t q_pi);
        set<set<Statement*>> FMax(function<set<set<Statement*>>(uint32_t , uint32_t)> F, uint32_t q_cfg, uint32_t q_pi, NFA* cfg, DFA* proof);

        set<Trace> getCounterExamples(map<IntersectionState, map<Statement*, IntersectionState>>& inactivityProof, IntersectionState& initialState);
        void alphabetPowerSetGenerationHelper(unordered_set<Statement *>::const_iterator it,
                                                const Alphabet &alphabet,
                                                set<Statement*> tempSet,
                                                set<set<Statement*>>& powerSetSet
        );

        void alphabetPowerSetGenerationHelper(set<Statement *>::const_iterator it,
                                              const set<Statement*>& alphabet,
                                              set<Statement*> tempSet,
                                              set<set<Statement*>>& powerSetSet
        );

        Program* _program;
    };
}