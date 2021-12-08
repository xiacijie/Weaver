#include "NFA.h"
#include <algorithm>
#include <iostream>
#include <queue>
#include <cassert>
#include "../ast/ASTNode.h"

using namespace std;
using namespace weaver;

unordered_map<Statement*, unordered_set<uint32_t>>& NFA::getTransitions(uint32_t fromState) {
    assert(hasTransitionFrom(fromState) && "Error");
    return _transitionTable[fromState];
}

NFA* NFA::shuffle(NFA *other, Alphabet &alphabet) {
    NFA* result = new NFA();

    // state products
    map<pair<uint32_t, uint32_t>, uint32_t> productMap;
    uint32_t newState = 0;
    for (const auto& s1: _states) {
        for (const auto& s2: other->getStates()) {
            productMap[make_pair(s1,s2)] = newState;
            result->addState(newState);

            if (isStartState(s1) && other->isStartState(s2)) {
                result->setStartState(newState);
            }

            if (isAcceptState(s1) && other->isAcceptState(s2)) {
                result->setAcceptState(newState);
            }

            newState++;
        }
    }

    // set transitions
    for (const auto& s1: _states) {
        for (const auto& s2: other->getStates()) {
            auto p = make_pair(s1,s2);
            uint32_t fromState = productMap[p];

            for (const auto& alpha: alphabet) {

                if (hasTransition(s1, alpha)) {
                    const unordered_set<uint32_t>& s1TargetStates = getTargetStates(s1, alpha);
                    for (const auto& t: s1TargetStates) {
                        result->addTransition(fromState, alpha, productMap[make_pair(t,  s2)]);
                    }
                }

                if (other->hasTransition(s2, alpha)) {
                    const unordered_set<uint32_t>& s2TargetStates = other->getTargetStates(s2, alpha);
                    for (const auto& t: s2TargetStates) {
                        result->addTransition(fromState, alpha, productMap[make_pair(s1, t)]);
                    }
                }
            }

            // handle epsilon transitions
            if (hasTransition(s1, nullptr)) {
                const unordered_set<uint32_t>& s1TargetStates = getTargetStates(s1, nullptr);
                for (const auto& t: s1TargetStates) {
                    result->addTransition(fromState, nullptr, productMap[make_pair(t,  s2)]);
                }

                //result->addTransition(fromState, nullptr, productMap[make_pair(s1, s2)]);
            }

            if (other->hasTransition(s2, nullptr)) {
                const unordered_set<uint32_t>& s2TargetStates = other->getTargetStates(s2, nullptr);
                for (const auto& t: s2TargetStates) {
                    result->addTransition(fromState, nullptr, productMap[make_pair(s1, t)]);
                }

                //result->addTransition(fromState, nullptr, productMap[make_pair(s1, s2)]);
            }
        }
    }


    return result;
}

void NFA::concatenate(uint32_t linkState, NFA *other) {
    assert(hasState(linkState) && "Error");

    uint32_t stateOffset = getNumStates();

    for (const auto& s: other->getStates()) {
        addState(s + stateOffset);
    }

    uint32_t otherStartState = other->getStartState();

    addTransition(linkState, nullptr, otherStartState + stateOffset);

    // set the transitions
    for (const auto& it : other->_transitionTable) {
        uint32_t otherFromState = it.first;

        for (const auto& it1 : it.second) {
            Statement* statement = it1.first;
            for (const auto& otherToState: it1.second) {
                addTransition(otherFromState + stateOffset, statement, otherToState + stateOffset);
            }
        }
    }
}

void NFA::removeUnreachableStates() {
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
                for (const auto&s: t.second) {
                    neighbours.push_back(s);
                }

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

DFA* NFA::convertToDFA(Alphabet& alphabet) {
    removeUnreachableStates();

    DFA* automata = new DFA();

    map<set<uint32_t>, uint32_t> powerSetMap;
    set<uint32_t> tempSet;

    uint32_t newState = 0;

    set<set<uint32_t>> powerSetSet;
    powerSetGenerationHelper(_states.begin() ,tempSet, powerSetSet, powerSetMap, newState, automata);

    automata->setStartState(powerSetMap[epsilonClosure(_startState)]);

    for (const auto& dfaFromStateSet: powerSetSet) {

        for (const auto& alpha: alphabet) {
            set<uint32_t> dfaToStateSet;
            for (const auto& s: dfaFromStateSet) {
                if (hasTransition(s, alpha)) {
                    auto closure = epsilonClosure(_transitionTable[s][alpha]);
                    dfaToStateSet.insert(closure.begin(), closure.end());
                }
            }

            uint32_t dfaFromState = powerSetMap[dfaFromStateSet];
            uint32_t dfaToState = powerSetMap[dfaToStateSet];
            automata->addTransition(dfaFromState, alpha, dfaToState);
        }
    }

    return automata;
}

set<uint32_t> NFA::epsilonClosure(unordered_set<uint32_t>& states) {
    set<uint32_t> eClosure;
    for (const auto& s : states) {
        auto c = epsilonClosure(s);
        eClosure.insert(c.begin(), c.end());
    }

    return eClosure;
}

set<uint32_t> NFA::epsilonClosure(uint32_t state) {
    set<uint32_t> eClosure;
    queue<uint32_t> q;
    unordered_set<uint32_t> visited;

    eClosure.insert(state);
    q.push(state);

    while (!q.empty()) {
        uint32_t currentState = q.front();
        visited.insert(currentState);
        q.pop();

        if (hasTransitionFrom(currentState)) {
            for (const auto& t : _transitionTable[currentState]) {
                if (t.first == nullptr) { // epsilon transition
                    for (const auto& s: t.second) {
                        if (visited.find(s) == visited.end()) {
                            eClosure.insert(s);
                            q.push(s);
                        }
                    }
                }
            }
        }

    }

    return eClosure;
}

void NFA::powerSetGenerationHelper(unordered_set<uint32_t>::iterator it,
                                   set<uint32_t> tempSet,
                                   set<set<uint32_t>>& powerSetSet,
                                   map<set<uint32_t>, uint32_t>& powerSetMap,
                                   uint32_t& newState,
                                   DFA* automata) {
    powerSetSet.insert(tempSet);
    powerSetMap[tempSet] = newState;
    automata->addState(newState);

    for (const auto& acceptState : _acceptStates) {
        if (tempSet.find(acceptState) != tempSet.end()) {
            automata->setAcceptState(newState);
            break;
        }
    }

    newState++;

    if (it == _states.end())
        return;

    for (auto localIt = it; localIt != _states.end(); localIt++) {
        tempSet.insert(*localIt);
        powerSetGenerationHelper(++it, tempSet, powerSetSet,powerSetMap, newState, automata);
        const auto& it1 = tempSet.find(*localIt);
        tempSet.erase(it1);
    }

}

const unordered_set<uint32_t>& NFA::getTargetStates(uint32_t state, Statement *statement) {
    assert(hasTransition(state, statement) && "Error!\n");
    return _transitionTable[state][statement];
}

bool NFA::hasTransitionFrom(uint32_t state) {
    const auto& it = _transitionTable.find(state);
    return it != _transitionTable.end();
}

bool NFA::hasTransitionTo(uint32_t state) {
    for (const auto& it : _transitionTable){
        bool any = any_of(it.second.begin(), it.second.end(), [=](const auto& it1) {
            return any_of(it1.second.begin(), it1.second.end(), [=](const auto& it2) {
                return it2 == state;
            });
        });

        if (any)
            return true;
    }

    return false;
}

bool NFA::hasTransition(uint32_t fromState, Statement *statement) {
    if (!hasTransitionFrom(fromState))
        return false;

    return _transitionTable[fromState].find(statement) != _transitionTable[fromState].end();
}

bool NFA::isAdjacent(uint32_t fromState, uint32_t toState) {
    if (!hasTransitionFrom(fromState))
        return false;

    return any_of(_transitionTable[fromState].begin(), _transitionTable[fromState].end(), [=](const auto& it) {
        return any_of(it.second.begin(), it.second.end(), [=](const auto& it1) {
            return it1 == toState;
        });
    });
}

string NFA::toString() {
    stringstream ss;

    ss << "NFA:" << endl;
    ss << "|============|" << endl;
    ss << "Num of Total States: " << getNumStates() << endl;
    ss << "Start State: " << getStartState() << endl;
    ss << "Accept States: " << getNumAcceptingStates() << " States" << endl;
    for (const auto& elem: _acceptStates) {
        ss << elem << " ";
    }
    ss << endl;
    ss << "Transitions: " << endl;

    for (const auto& t: _transitionTable) {
        for (const auto& it : t.second) {
            string statement;

            if (it.first == nullptr) {
                statement = "NULL";
            }
            else {
                statement = it.first->toString();
            }

            ss << "( " << t.first << ", " << statement << ", " << "{";

            int i = 0;
            for (const auto& it1: it.second) {
                ss << it1;
                if (i != it.second.size() - 1) {
                    ss <<", ";
                }
                i++;
            }

            ss << "} )";
        }

        ss << endl;
    }
    ss << "|============|" << endl;

    return ss.str();
}

void NFA::addTransition(uint32_t fromState, Statement *statement, uint32_t toState) {
    assert(hasState(fromState) && "From State does not exist!\n");
    assert(hasState(toState) && "To State does not exist!\n");

//    if (hasTransition(fromState, statement)) {
//        if (_transitionTable[fromState][statement].find(toState) != _transitionTable[fromState][statement].end()) {
//            assert(false && "This transition already exists!\n");
//        }
//    }

    _transitionTable[fromState][statement].insert(toState);
}

void NFA::removeTransition(uint32_t fromState, Statement *statement, uint32_t toState) {
    assert(hasState(fromState) && "From State does not exist!\n");
    assert(hasState(toState) && "To State does not exist!\n");

    if (hasTransition(fromState, statement)) {
        const auto& it = _transitionTable[fromState][statement].find(toState);
        if (it == _transitionTable[fromState][statement].end()) {
            assert(false && "Transition does not exit!\n");
        }

        _transitionTable[fromState][statement].erase(it);
    }

    assert(false && "Transition does not exit!\n");
}

