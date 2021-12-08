#include "LoopingTreeAutomata.h"
#include <tuple>
#include <algorithm>
#include <iostream>
#include <cassert>

using namespace std;
using namespace weaver;

LoopingTreeAutomata::LoopingTreeAutomata(DFA *dfa, const unordered_map<Statement *, unordered_set<Statement *>> &dependenceRelation,
                                         const Alphabet &alphabet) {

    set<Statement*> tempSet;
    set<set<Statement*>> powerSetSet;

    alphabetPowerSetGenerationHelper(alphabet.begin(), alphabet, tempSet, powerSetSet);

    map<tuple<uint32_t, bool, set<Statement*>>, uint32_t> productMap;
    uint32_t newState = 0;

    // set the states
    for (const auto& state: dfa->getStates()) {
        for (const auto& alphaSet: powerSetSet) {
            // B: true
            auto t1 = make_tuple(state, true, alphaSet);
            productMap[t1] = newState;
            _states.insert(newState);
            newState++;

            //B: false
            auto t2 = make_tuple(state, false, alphaSet);
            productMap[t2] = newState;
            _states.insert(newState);

            // set the start state
            if (dfa->isStartState(state) && alphaSet.empty()) {
                _startState = newState;
            }

            newState++;
        }
    }

    // set the transitions
    vector<Statement*> alphabetPermutation(alphabet.begin(), alphabet.end());
    sort(alphabetPermutation.begin(), alphabetPermutation.end());

    for (const auto& it : productMap) {
        // each state
        uint32_t q = get<uint32_t>(it.first);
        bool i =  get<bool>(it.first);
        const set<Statement*>& S = get<set<Statement*>>(it.first);
        uint32_t state = it.second;

        // all the linear orderings of alphabet
        do {
            for (const auto& a: alphabet) {
                set<Statement*> S_union_R_a;

                auto pos = find(alphabetPermutation.begin(), alphabetPermutation.end(), a);
                if (pos == alphabetPermutation.end()) {
                    assert(false && "Error\n");
                }

                set<Statement*>R_a(pos, alphabetPermutation.end());

                set_union(S.begin(), S.end(), R_a.begin(), R_a.end(), inserter(S_union_R_a, S_union_R_a.begin()));

                set<Statement*> S_union_R_a_minus_D_a;
                set_difference(S_union_R_a.begin(), S_union_R_a.end(),
                               dependenceRelation.at(a).begin(), dependenceRelation.at(a).end(),
                               inserter(S_union_R_a_minus_D_a, S_union_R_a_minus_D_a.begin())
                               );

                auto t = make_tuple(dfa->getTargetState(q, a), (i || S.find(a) != S.end()), S_union_R_a_minus_D_a);
                auto it1 = productMap.find(t);

                if (it1 == productMap.end()) {
                    assert(false && "Error\n");
                }

                _transitionTable[state][dfa->isAcceptState(q) && !i][a] = it1->second;
            }
        } while (next_permutation(alphabetPermutation.begin(), alphabetPermutation.end()));

    }
}

LoopingTreeAutomata::LoopingTreeAutomata(DFA *dfa, Alphabet &alphabet) {
    for (const auto& state: dfa->getStates()) {
        addState(state);
        if (dfa->isStartState(state)) {
            _startState = state;
        }

        for (const auto& alpha: alphabet) {
            if (dfa->isAcceptState(state)) {
                _transitionTable[state][true][alpha] = dfa->getTargetState(state, alpha);
            }

            _transitionTable[state][false][alpha] = dfa->getTargetState(state, alpha);
        }

    }

}

void LoopingTreeAutomata::alphabetPowerSetGenerationHelper(unordered_set<Statement *>::const_iterator it, const Alphabet &alphabet,
                                                           set<Statement *> tempSet, set<set<Statement*>> &powerSetSet) {
    powerSetSet.insert(tempSet);

    if (it == alphabet.end())
        return;

    for (auto localIt = it; localIt != alphabet.end(); localIt++) {
        tempSet.insert(*localIt);
        alphabetPowerSetGenerationHelper(++it, alphabet, tempSet, powerSetSet);
        const auto& it1 = tempSet.find(*localIt);
        tempSet.erase(it1);
    }
}

LoopingTreeAutomata* LoopingTreeAutomata::intersect(LoopingTreeAutomata *other, Alphabet &alphabet) {
    LoopingTreeAutomata* result = new LoopingTreeAutomata();

    map<pair<uint32_t, uint32_t>, uint32_t> productMap;

    //set states
    uint32_t newState = 0;

    for (const auto& s1 : _states) {
        for (const auto& s2: other->_states) {
            productMap[make_pair(s1,s2)] = newState;
            result->addState(newState);

            if (isStartState(s1) && result->isStartState(s2)) {
                result->setStartState(newState);
            }

            newState++;

        }
    }

    //set transitions
    for (const auto& it : productMap) {
        uint32_t q1 = it.first.first;
        uint32_t q2 = it.first.second;

        uint32_t state = it.second;

        // B == true
        if (_transitionTable[q1].find(true) != _transitionTable[q1].end() &&
            other->_transitionTable[q2].find(true) != other->_transitionTable[q2].end()) {

            for (const auto & alpha: alphabet) {
                uint32_t q1To = _transitionTable[q1][true][alpha];
                uint32_t q2To = other->_transitionTable[q2][true][alpha];
                result->_transitionTable[state][true][alpha] = productMap[make_pair(q1To, q2To)];
            }
        }


        // B == false
        if (_transitionTable[q1].find(false) != _transitionTable[q1].end() &&
            other->_transitionTable[q2].find(false) != other->_transitionTable[q2].end()) {

            for (const auto & alpha: alphabet) {
                uint32_t q1To = _transitionTable[q1][true][alpha];
                uint32_t q2To = other->_transitionTable[q2][true][alpha];
                result->_transitionTable[state][true][alpha] = productMap[make_pair(q1To, q2To)];
            }
        }

    }

    return result;
}

void LoopingTreeAutomata::setStartState(uint32_t state) {
    assert(hasState(state) && "Has no such state!\n");

    _startState = state;
}