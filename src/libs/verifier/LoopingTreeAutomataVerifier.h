#pragma once
#include "Program.h"
#include <functional>
#include "Set.h"
#include "AntiChain.h"
#include "VerifierBase.h"

namespace weaver {
    class LoopingTreeAutomataVerifier : public VerifierBase {
    public:
        explicit LoopingTreeAutomataVerifier(Program* program):
            VerifierBase(program)
        {}

        /**
         *
         * @return true if the program is correct.
         * false if the program is incorrect
         */
        bool verify() override;

    private:
        typedef tuple<uint32_t, bool, set<Statement*>> LTAState;
        typedef pair<LTAState, uint32_t> IntersectionState;
        typedef tuple<uint32_t, bool, uint32_t> T;

        set<Trace> proofCheck(NFA* cfg, DFA* proof);
        set<Trace> getCounterExamples(const map<IntersectionState, map<Statement*, IntersectionState>>& inactivityProof,
                                      IntersectionState& initialState);

        set<Trace> proofCheckWithAntiChains(NFA* cfg, DFA* proof);
        void addToInactivityProof(map<T, map<set<Statement*>, map<Statement*, IntersectionState>>>& inactivityProof,
                                  T& fromState, const set<Statement*>& S, Statement* statement, IntersectionState& toState);

        set<Trace> getCounterExamples(const  map<T, map<set<Statement*>, map<Statement*, IntersectionState>>>& inactivityProof,
                                      T& initialState);

        string inactivityProofToString(const map<IntersectionState, map<Statement*, IntersectionState>>& inactivityProof);
        string inactivityProofToString(const  map<T, map<set<Statement*>, map<Statement*, IntersectionState>>>& inactivityProof);

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
    };
}