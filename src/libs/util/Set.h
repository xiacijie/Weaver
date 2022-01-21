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

    template<typename T>
    void powerSetHelper(size_t k, 
        typename set<T>::const_iterator it, 
        const set<T>& s, 
        set<T> temp, 
        set<set<T>>& result) {
        
        if (temp.size() == k) {
            result.insert(temp);
            return;
        }

        while (it != s.end()) {
            T current = *it;
            it++;
            temp.insert(current);
            powerSetHelper(k, it, s, temp, result);
            temp.erase(current);
        }
    } 

    template<typename T>
    void powerSet(const set<T>& s, set<set<T>>& result) {
        assert(result.empty() && "result is not empty!");
        
        for (int i = 0; i <= s.size(); i++) {
            powerSetHelper(i, s.begin(), s, {}, result);
        }
    }
}
