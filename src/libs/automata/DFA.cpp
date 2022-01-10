#include "DFA.h"
#include "NFA.h"
#include <algorithm>
#include <memory>
#include <iostream>
#include <queue>
#include <cassert>
#include "ASTNode.h"

using namespace weaver;
using namespace std;

bool DFA::isComplete(Alphabet& alphabet) {
    int totalTransitions = 0;
    for (const auto& t : _transitionTable) {
        totalTransitions += t.second.size();
    }

    return totalTransitions == alphabet.size() * _states.size();
}

DFA* DFA::shuffle(DFA *other, Alphabet &alphabet) {
    assert(other->isComplete(alphabet) && isComplete(alphabet) && "Cannot shuffle incomplete DFAs!\n");

    NFA* nfa = new NFA();

    uint32_t newState = 0;
    map<pair<uint32_t, uint32_t>, uint32_t> productMap;

    // set states
    for (const auto& s1: _states) {
        for (const auto& s2: other->_states) {
            nfa->addState(newState);

            productMap[make_pair(s1,s2)] = newState;

            if (isStartState(s1) && other->isStartState(s2)) {
                nfa->setStartState(newState);
            }

            if (isAcceptState(s1) && other->isAcceptState(s2)) {
                nfa->setAcceptState(newState);
            }

            newState++;
        }
    }

    // set transitions
    for (const auto& s1 : _states) {
        for (const auto& s2 : other->_states) {
            for (const auto& alpha: alphabet) {
                uint32_t s1ToState = _transitionTable[s1][alpha];
                uint32_t s2ToState = other->_transitionTable[s2][alpha];
                auto p = make_pair(s1,s2);

                nfa->addTransition(productMap[p], alpha, productMap[make_pair(s1ToState, s2)]);
                nfa->addTransition(productMap[p], alpha, productMap[make_pair(s1, s2ToState)]);
            }

        }
    }

    DFA* result = nfa->convertToDFA(alphabet);
    delete nfa;

    return result;
}

void DFA::minimize(Alphabet& alphabet) {
    assert(isComplete(alphabet) && "Can only do minimization on complete DFA!\n");

    removeUnreachableStates();

    // using Myhill-Nerode Theorem, table filling method
    map<pair<uint32_t, uint32_t>, bool> markingTable;
    for (const auto& s1: _states) {
        for (const auto &s2 : _states) {
            if (s1 < s2) {
                auto p = make_pair(s1, s2);
                markingTable[p] = isAcceptState(s1) != isAcceptState(s2);

                }
            }
        }


    while (true) {
        bool newMarkingMade = false;

        for (const auto& t : markingTable) {
            if (t.second == false) {
                uint32_t s1 = t.first.first;
                uint32_t s2 = t.first.second;
                for (const auto& alpha: alphabet) {
                    uint32_t s1To = _transitionTable[s1][alpha];
                    uint32_t s2To = _transitionTable[s2][alpha];
                    pair<uint32_t, uint32_t> toPair;

                    if (s1To < s2To) {
                        toPair = make_pair(s1To, s2To);
                    }
                    else if (s1To > s2To) {
                        toPair = make_pair(s2To, s1To);
                    }
                    else {
                        continue;
                    }

                    if (markingTable[toPair] == true) { // marked
                        markingTable[t.first] = true;
                        newMarkingMade = true;
                    }
                }
            }

        }

        if (!newMarkingMade)
            break;
    }


    set<set<uint32_t>> mergeStateSet;

    for (const auto& t : markingTable) {
        auto& p = t.first;
        uint32_t s1 = p.first;
        uint32_t s2 = p.second;

        if (t.second == false) { // not marked, need merging

            bool matchedSetFound = false;

            for (const auto& it : mergeStateSet) {
                if (it.find(s1) != it.end()) {
                    set<uint32_t> newSet(it);
                    newSet.insert(s2);

                    mergeStateSet.erase(it);
                    mergeStateSet.insert(newSet);

                    matchedSetFound = true;
                    break;
                }
                else if (it.find(s2) != it.end()) {
                    set<uint32_t> newSet(it);
                    newSet.insert(s1);

                    mergeStateSet.erase(it);
                    mergeStateSet.insert(newSet);

                    matchedSetFound = true;
                    break;
                }
            }

            if (!matchedSetFound) {
                mergeStateSet.insert(set<uint32_t>{s1, s2});
            }
        }
        else { // do not merge
            mergeStateSet.insert(set<uint32_t>{s1});
            mergeStateSet.insert(set<uint32_t>{s2});
        }
    }

    //turn it into a map
    map<set<uint32_t>, uint32_t> mergeStateSetMap;
    uint32_t newState = 0;

    for (const auto& it : mergeStateSet) {
        mergeStateSetMap[it] = newState++;
    }

    unordered_map<uint32_t, unordered_map<Statement*, uint32_t>> newTransitionTable;
    unordered_set<uint32_t> newAcceptStates;

    bool newStartStateSet = false;

    uint32_t newStartState;

    _states.clear();

    for (const auto& it : mergeStateSetMap) {
        _states.insert(it.second);

        // set start state
        if (!newStartStateSet && it.first.find(_startState) != it.first.end()) {
            newStartState = it.second;
            newStartStateSet = true;
        }

        //set accepting state
        for (const auto& state: _acceptStates) {
            if (it.first.find(state) != it.first.end()) {
                newAcceptStates.insert(it.second);
                break;
            }
        }

        //set transitions
        for (const auto& alpha: alphabet) {
            uint32_t oldFromState = *it.first.begin();
            uint32_t oldToState = _transitionTable[oldFromState][alpha];

            uint32_t newFromState = it.second;
            uint32_t newToState;
            for (const auto & it1 : mergeStateSetMap) {
                if (it1.first.find(oldToState) != it1.first.end()) {
                    newToState = it1.second;
                    break;
                }
            }

            newTransitionTable[newFromState][alpha] = newToState;
        }
    }

    if (newStartStateSet == false) {
        assert(false && "Start state not set!\n");
    }

    _acceptStates.clear();
    _acceptStates.insert(newAcceptStates.begin(), newAcceptStates.end());

    _startState = newStartState;

    _transitionTable.clear();
    _transitionTable.insert(newTransitionTable.begin(), newTransitionTable.end());

    removeUnreachableStates();
}

void DFA::removeUnreachableStates() {

    vector<uint32_t> statesToRemove;

    for (const auto& state: _states) {
        if (state != _startState && !hasTransitionTo(state)) {
            statesToRemove.push_back(state);
        }
    }

    while (!statesToRemove.empty()) {
        vector<uint32_t> neighbours;

        for (const auto& state: statesToRemove) {
            if (!hasState(state))
                continue;

            for (const auto& t: _transitionTable[state]) {
                neighbours.push_back(t.second);
            }

            _transitionTable.erase(_transitionTable.find(state));
            removeState(state);
        }

        statesToRemove.clear();

        for (const auto& state: neighbours) {
            if (state != _startState && !hasTransitionTo(state)) {
                statesToRemove.push_back(state);
            }
        }
    }

}


void DFA::complete(uint32_t garbageState, Alphabet& alphabet) {
    if (isComplete(alphabet))
        return;

    addState(garbageState);

    for (const auto& state : _states) {
        if (!hasTransitionFrom(state)) {
            for (const auto& statement: alphabet) {
                addTransition(state, statement, garbageState);
            }
        }
        else {
            for (const auto& statement: alphabet) {
                // does not have this transition, make a transition to garbage state
                if (!hasTransition(state, statement)) {
                    addTransition(state, statement, garbageState);
                }
            }
        }
    }

}

DFA* DFA::complement(Alphabet& alphabet) {
    assert(isComplete(alphabet) && "Cannot do complement on an incomplete Automata!\n");
    DFA* result = new DFA();

    //copy the transitions
    result->_transitionTable.insert(_transitionTable.begin(), _transitionTable.end());

    //copy the states
    result->_states.insert(_states.begin(), _states.end());

    //copy the start state
    result->_startState = getStartState();

    // swap the accepting states and non-accepting states.
    for (const auto& state : _states) {
        auto it = _acceptStates.find(state);
        if (it == _acceptStates.end()) { // if it is a non-accepting state
           result->setAcceptState(state);
        }
    }

    return result;
}

DFA* DFA::intersect(DFA *other, Alphabet& alphabet) {
    assert((isComplete(alphabet) && other->isComplete(alphabet)) && "Cannot do intersections on incomplete DFAs!\n");

    DFA* result = new DFA();

    //take the product of two states
    map<pair<uint32_t, uint32_t>,uint32_t> productMapping;

    uint32_t currentState = 0;
    for (const auto& state1: _states) {
        for (const auto& state2: other->_states) {
            productMapping[make_pair(state1, state2)] = currentState;

            result->addState(currentState);

            if (isStartState(state1) && other->isStartState(state2)) { // both start states
                result->setStartState(currentState);
            }
            else if (isAcceptState(state1) && other->isAcceptState(state2)) { // both accept states
                result->setAcceptState(currentState);
            }

            currentState ++;
        }
    }

    //handle the transitions
    unordered_map<uint32_t,unordered_map<Statement*, uint32_t>> tempTransitionTable;
    for (const auto& state1: _states) {
        for (const auto& state2: other->_states) {
            for (auto alpha: alphabet) {
                uint32_t targetState1 = getTargetState(state1, alpha);
                uint32_t targetState2 = other->getTargetState(state2, alpha);

                result->addTransition(productMapping[make_pair(state1, state2)],
                                     alpha,
                                     productMapping[make_pair(targetState1, targetState2)]);

            }
        }
    }
    return result;
}

DFA* DFA::Union(DFA *other, Alphabet &alphabet) {
    assert((isComplete(alphabet) && other->isComplete(alphabet)) && "Cannot do union on incomplete DFAs!\n");

    DFA* result = new DFA();

    //take the product of two states
    map<pair<uint32_t, uint32_t>,uint32_t> productMapping;

    uint32_t currentState = 0;
    for (const auto& state1: _states) {
        for (const auto& state2: other->_states) {
            productMapping[make_pair(state1, state2)] = currentState;

            result->addState(currentState);

            if (isStartState(state1) && other->isStartState(state2)) { // both start states
                result->setStartState(currentState);
            }
            else if (isAcceptState(state1) || other->isAcceptState(state2)) { // any is accepting states
                result->setAcceptState(currentState);
            }

            currentState ++;
        }
    }

    //handle the transitions
    unordered_map<uint32_t,unordered_map<Statement*, uint32_t>> tempTransitionTable;
    for (const auto& state1: _states) {
        for (const auto& state2: other->_states) {
            for (auto alpha: alphabet) {
                uint32_t targetState1 = getTargetState(state1, alpha);
                uint32_t targetState2 = other->getTargetState(state2, alpha);

                result->addTransition(productMapping[make_pair(state1, state2)],
                                      alpha,
                                      productMapping[make_pair(targetState1, targetState2)]);

            }
        }
    }
    return result;
}

string DFA::toString(Alphabet& alphabet) {
    stringstream ss;

    ss << "DFA:" << endl;
    ss << "|============|" << endl;
    string complete = "FALSE";
    if (isComplete(alphabet)) {
        complete = "TRUE";
    }
    ss << "Is Automata complete? " << complete << endl;
    ss << "Num of Total States: " << getNumStates() << endl;
    ss << "Start State: " << getStartState() << endl;
    ss << "Accept States: " << getNumAcceptingStates() << " States" << endl;
    for (const auto& elem: _acceptStates) {
        ss << elem << " ";
    }
    ss << endl;
    ss << "Transitions: " << endl;

    for (const auto &t: _transitionTable) {
        for (const auto& it : t.second) {
            ss << "( " << t.first << ", " <<
                                          (it.first ? it.first->toString() : "null") << ", " << it.second << " )";
        }

        ss << endl;
    }
    ss << "|============|" << endl;

    return ss.str();
}

bool DFA::isAdjacent(uint32_t fromState, uint32_t toState) {
    if (!hasTransitionFrom(fromState))
        return false;

    return any_of( _transitionTable[fromState].begin(),  _transitionTable[fromState].end(), [=](const auto& it) {
        return it.second == toState;
    });
}

bool DFA::hasTransition(uint32_t fromState, Statement *statement) {
    if (!hasTransitionFrom(fromState)) {
        return false;
    }

    return _transitionTable[fromState].find(statement) != _transitionTable[fromState].end();
}

bool DFA::hasTransition(uint32_t fromState, Statement *statement, uint32_t toState) {
    if (!hasTransition(fromState, statement)) {
        return false;
    }

    return _transitionTable[fromState][statement] == toState;
}

bool DFA::hasTransitionFrom(uint32_t state) {
    const auto& it = _transitionTable.find(state);
    return it != _transitionTable.end();
}

bool DFA::hasTransitionTo(uint32_t state) {
    for (const auto& it : _transitionTable) {
        bool any = any_of(it.second.begin(), it.second.end(), [=](const auto& it1) {
           return it1.second == state;
        });

        if (any)
            return true;
    }

    return false;
}

uint32_t DFA::getTargetState(uint32_t fromState, Statement *statement) {
    assert(hasTransition(fromState, statement) && "No transition from fromState!\n");
    return _transitionTable[fromState][statement];
}

unordered_map<Statement*, uint32_t>& DFA::getTransitions(uint32_t fromState) {
    assert(hasTransitionFrom(fromState) && "Has no transitions!\n");

    return _transitionTable[fromState];
}

void DFA::addTransition(uint32_t fromState, Statement *statement, uint32_t toState) {
    assert(hasState(fromState) && "From State does not exist!\n");
    assert(hasState(toState) && "To State does not exist!\n");

    if (hasTransition(fromState, statement)) {
        // add transition already there
        if (getTargetState(fromState, statement) == toState)
            return;
        assert(false && "Already has transition with the from state and the statement!\n");
    }

    _transitionTable[fromState][statement] = toState;
}

void DFA::removeTransition(uint32_t fromState, Statement *statement, uint32_t toState) {
    assert(hasState(fromState) && "From State does not exist!\n");
    assert(hasState(toState) && "To State does not exist!\n");

    if (hasTransition(fromState, statement)) {
        if (_transitionTable[fromState][statement] != toState) {
            assert(false && "This transition does not exist!\n");
        }
    }

    _transitionTable[fromState].erase(_transitionTable[fromState].find(statement));
}