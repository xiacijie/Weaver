#include "AntiChain.h"
#include "Statement.h"
#include "Set.h"
#include <cassert>

using namespace std;
using namespace weaver;

template<typename T>
void util::antiChainMeet(const set<set<T>>& left, const set<set<T>>& right, set<set<T>>& result) {

}

template<typename T>
void util::antiChainJoin(const set<set<T>>& left, const set<set<T>>& right, set<set<T>>& result) {

}

template<typename T>
void util::antiChainMax(const set<set<T>>& s, set<set<T>>& result) {
    assert(result.empty() && "result is not empty!");
    for (const auto& i: s) {
        bool currentSetIsMax = true;

        set<set<Statement*>> setToRemoveFromResult;

        for (const auto& max: result) {
            if (max == i || setInclusion(i, max)) {
                currentSetIsMax = false;
                break;
            }

            // i will be the new max
            if (setInclusion(max, i)) {
                setToRemoveFromResult.insert(max);
            }
        }

        if (!setToRemoveFromResult.empty()) {
            for (const auto& toRemove : setToRemoveFromResult) {
                result.erase(toRemove);
            }
        }

        if (currentSetIsMax) {
            result.insert(i);
        }
    }
}


template void util::antiChainMeet(const set<set<Statement*>>& left, const set<set<Statement*>>& right, set<set<Statement*>>& result);
template void util::antiChainJoin(const set<set<Statement*>>& left, const set<set<Statement*>>& right, set<set<Statement*>>& result);
template void util::antiChainMax(const set<set<Statement*>>& s, set<set<Statement*>>& result);