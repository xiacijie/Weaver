#pragma once
#include "FiniteAutomataBase.h"
#include <map>
#include <unordered_map>
#include "String.h"


using namespace std;

namespace weaver {
    class DFA : public FiniteAutomataBase {
    public:
        DFA() : FiniteAutomataBase() {}

        void addTransition(uint32_t fromState, Statement* statement, uint32_t toState) override;
        void removeTransition(uint32_t fromState, Statement* statement, uint32_t toState) override;
        bool isAdjacent(uint32_t fromState, uint32_t toState) override;
        bool hasTransitionFrom(uint32_t state) override;
        bool hasTransitionTo(uint32_t state) override;
        bool hasTransition(uint32_t fromState, Statement* statement) override;
        bool hasTransition(uint32_t fromState, Statement* statement, uint32_t toState) override;
        string toString(Alphabet& alphabet);

        uint32_t getTargetState(uint32_t fromState, Statement* statement);

        void complete(uint32_t garbageState, Alphabet& alphabet);
        bool isComplete(Alphabet& alphabet);

        DFA* complement(Alphabet& alphabet);
        DFA* intersect(DFA* other, Alphabet& alphabet);
        DFA* Union(DFA* other, Alphabet& alphabet);
        DFA* shuffle(DFA* other, Alphabet& alphabet);

        void minimize(Alphabet& alphabet);
        void removeUnreachableStates() override;

        unordered_map<Statement*, uint32_t>& getTransitions(uint32_t fromState);

    private:
        unordered_map<uint32_t, unordered_map<Statement*, uint32_t>> _transitionTable;

    };

}
