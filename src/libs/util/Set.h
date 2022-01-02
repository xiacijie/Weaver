#pragma once
#include <set>
#include <algorithm>

using namespace std;

namespace util {
    template<typename T>
    void setDifference(const set<T>& left, const set<T>& right, set<T>& result) {
        assert(result.empty() && "result is not empty!");
        set_difference(left.begin(), left.end(),
                       right.begin(),right.end(),
                       inserter(result,result.begin()));
    }

    template<typename T>
    void setUnion(const set<T>& left, const set<T>& right, set<T>& result) {
        assert(result.empty() && "result is not empty!");
        set_union(left.begin(), left.end(),
                  right.begin(), right.end(),
                  inserter(result, result.begin()));
    }

    template<typename T>
    void setIntersection(const set<T>& left, const set<T>& right, set<T>& result) {
        assert(result.empty() && "result is not empty!");
        set_intersection(left.begin(),left.end(),
                         right.begin(), right.end(),
                         inserter(result, result.begin()));
    }

    template<typename T>
    bool setInclusion(const set<T>& left, const set<T>& right) {
        set<T> temp;
        util::setDifference(left, right, temp);
        return temp.empty();
    }
}
