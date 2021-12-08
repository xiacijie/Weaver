#pragma once
#include <unordered_set>
#include <unordered_map>
#include "../program/Statement.h"
#include "DFA.h"

using namespace std;

namespace weaver {
    class LoopingTreeAutomata {
    public:
        /**
         * Construct the LTA from the DFA given the independence relation
         *
         * @param dfa
         * @param independenceRelation
         * @param alphabet
         */
        LoopingTreeAutomata(DFA* dfa, const unordered_map<Statement*, unordered_set<Statement*>>& dependenceRelation, const Alphabet& alphabet);

        /**
         * Construct the power set of the language of the DFA
         *
         * @param dfa
         * @param alphabet
         */
        LoopingTreeAutomata(DFA* dfa, Alphabet& alphabet);

        LoopingTreeAutomata() {};

        LoopingTreeAutomata* intersect(LoopingTreeAutomata* other, Alphabet& alphabet);

        void addState(uint32_t state) { _states.insert(state); }
        bool hasState(uint32_t state) { return _states.find(state) != _states.end(); }
        bool isStartState(uint32_t state) { return state == _startState; }

        void setStartState(uint32_t state);

    private:
        /**
         * Get the power set of the alphabet
         */
        static void alphabetPowerSetGenerationHelper(unordered_set<Statement *>::const_iterator it,
                                                     const Alphabet &alphabet,
                                                     set<Statement*> tempSet,
                                                     set<set<Statement*>>& powerSetSet
                                                     );


        unordered_set<uint32_t> _states;
        uint32_t _startState;
        unordered_map<uint32_t, unordered_map<bool, unordered_map<Statement*, uint32_t>>> _transitionTable;

    };
}