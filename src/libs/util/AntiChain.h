#pragma once
#include <set>
using namespace std;

namespace util {
    template< typename T>
    void antiChainMeet(const set<set<T>>& left, const set<set<T>>& right, set<set<T>>& result);

    template< typename T>
    void antiChainJoin(const set<set<T>>& left, const set<set<T>>& right, set<set<T>>& result);

    template< typename T>
    void antiChainMax(const set<set<T>>& s, set<set<T>>& result);
}