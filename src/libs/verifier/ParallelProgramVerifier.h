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
        typedef tuple<uint32_t, bool, set<Statement*>> LTAState;
        typedef pair<LTAState, uint32_t> IntersectionState;

        set<Trace> getErrorTraces(NFA* cfg, DFA* proof);
        set<Trace> getCounterExamples(map<IntersectionState, map<Statement*, IntersectionState>>& inactivityProof, IntersectionState& initialState);
        void alphabetPowerSetGenerationHelper(unordered_set<Statement *>::const_iterator it,
                                                const Alphabet &alphabet,
                                                set<Statement*> tempSet,
                                                set<set<Statement*>>& powerSetSet
        );

        Program* _program;
    };
}