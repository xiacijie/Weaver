#pragma once
#include <set>
#include <algorithm>
using namespace std;

namespace util {


    template< typename T>
    void antiChainMax(const set<set<T>>& s, set<set<T>>& result) {
        assert(result.empty() && "result is not empty!");
        for (const auto& i: s) {
            bool currentSetIsMax = true;

            set<set<T>> setToRemoveFromResult;

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

    template< typename T>
    void antiChainMeet(const set<set<T>>& left, const set<set<T>>& right, set<set<T>>& result) {
        set<set<T>> temp;
        for (const auto& l: left) {
            for (const auto& r: right) {
                set<T> intersection;
                setIntersection(l,r,intersection);
                temp.insert(intersection);
            }
        }

        antiChainMax(temp, result);
    }

    template< typename T>
    void antiChainJoin(const set<set<T>>& left, const set<set<T>>& right, set<set<T>>& result)  {
        set<set<T>> temp;
        setUnion(left, right, temp);
        antiChainMax(temp, result);
    }

}