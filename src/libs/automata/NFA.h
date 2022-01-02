#pragma once
#include "FiniteAutomataBase.h"
#include "DFA.h"
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include "String.h"


namespace weaver {
    class NFA : public FiniteAutomataBase {
    public:
        void addTransition(uint32_t fromState, Statement* statement, uint32_t toState) override;
        void removeTransition(uint32_t fromState, Statement* statement, uint32_t toState) override;

        bool isAdjacent(uint32_t fromState, uint32_t toState) override;
        bool hasTransitionFrom(uint32_t state) override;
        bool hasTransitionTo(uint32_t state) override;
        bool hasTransition(uint32_t fromState, Statement* statement) override;
        string toString();

        const unordered_set<uint32_t>& getTargetStates(uint32_t state, Statement* statement);
        void removeUnreachableStates() override;
        DFA* convertToDFA(Alphabet& alphabet);
        NFA* shuffle(NFA* other, Alphabet& alphabet);

        void concatenate(uint32_t linkState, NFA* other);

        unordered_map<Statement*, unordered_set<uint32_t>>& getTransitions(uint32_t fromState);

    private:
        // (from-state, statement, {to-states})
        unordered_map<uint32_t,unordered_map<Statement*, unordered_set<uint32_t>>> _transitionTable;
        void powerSetGenerationHelper(unordered_set<uint32_t>::iterator it,
                                      set<uint32_t> tempSet,
                                      set<set<uint32_t>>& powerSetSet,
                                      map<set<uint32_t>, uint32_t>& powerSetMap,
                                      uint32_t& newState,
                                      DFA* automata);

        set<uint32_t> epsilonClosure(uint32_t state);
        set<uint32_t> epsilonClosure(unordered_set<uint32_t>& states);
    };
}

