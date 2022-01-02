#pragma once
#include <set>
using namespace std;

namespace util {
    template<typename T>
    void setDifference(const set<T>& left, const set<T>& right, set<T>& result);

    template<typename T>
    void setUnion(const set<T>& left, const set<T>& right, set<T>& result);

    template<typename T>
    void setIntersection(const set<T>& left, const set<T>& right, set<T>& result);

    template<typename T>
    bool setInclusion(const set<T>& left, const set<T>& right);
}
