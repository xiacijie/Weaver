#include "Set.h"
#include "Statement.h"
#include <algorithm>
#include <cassert>

using namespace std;
using namespace weaver;

template<typename T>
void util::setDifference(const set<T>& left, const set<T>& right, set<T>& result) {
    assert(result.empty() && "result is not empty!");
    set_difference(left.begin(), left.end(),
                   right.begin(),right.end(),
                   inserter(result,result.begin()));
}

template<typename T>
void util::setUnion(const set<T>& left, const set<T>& right, set<T>& result) {
    assert(result.empty() && "result is not empty!");
    set_union(left.begin(), left.end(),
              right.begin(), right.end(),
              inserter(result, result.begin()));
}

template<typename T>
void util::setIntersection(const set<T>& left, const set<T>& right, set<T>& result) {
    assert(result.empty() && "result is not empty!");
    set_intersection(left.begin(),left.end(),
                     right.begin(), right.end(),
                     inserter(result, result.begin()));
}

template<typename T>
bool util::setInclusion(const set<T>& left, const set<T>& right) {
    set<Statement*> temp;
    util::setDifference(left, right, temp);
    return temp.empty();
}

template void util::setDifference(const set<Statement*>& left, const set<Statement*>& right, set<Statement*>& result);
template void util::setUnion(const set<Statement*>& left, const set<Statement*>& right, set<Statement*>& result);
template void util::setIntersection(const set<Statement*>& left, const set<Statement*>& right, set<Statement*>& result);
template bool util::setInclusion(const set<Statement*>& left, const set<Statement*>& right);