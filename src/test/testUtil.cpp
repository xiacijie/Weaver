#include <catch2/catch_test_macros.hpp>
#include "Set.h"
#include "AntiChain.h"
#include <set>

using namespace util;
using namespace std;


TEST_CASE( "Set difference 1", "[set]" ) {
    set<int> s1{1,2,3};
    set<int> s2{2,3};
    set<int> result;
    setDifference(s1,s2, result);
    REQUIRE(result == set<int>{1});
}

TEST_CASE( "Set difference 2", "[set]" ) {
    set<int> s1{1,2,3};
    set<int> s2{2,3};
    set<int> result;
    setDifference(s2,s1, result);
    REQUIRE(result == set<int>{});
}

TEST_CASE( "Set difference 3", "[set]" ) {
    set<int> s1{1,2,3};
    set<int> s2{};
    set<int> result;
    setDifference(s1,s2, result);
    REQUIRE(result == set<int>{1,2,3});
}

TEST_CASE( "Set difference 4", "[set]" ) {
    set<int> s1{1,2,3};
    set<int> s2{};
    set<int> result;
    setDifference(s2,s1, result);
    REQUIRE(result == set<int>{});
}

TEST_CASE( "Set difference 5", "[set]" ) {
    set<int> s1{};
    set<int> s2{};
    set<int> result;
    setDifference(s1,s2, result);
    REQUIRE(result == set<int>{});
}

TEST_CASE( "Set difference 6", "[set]" ) {
    set<int> s1{};
    set<int> s2{};
    set<int> result;
    setDifference(s2,s1, result);
    REQUIRE(result == set<int>{});
}

TEST_CASE( "Set union 1", "[set]" ) {
    set<int> s1{1,2};
    set<int> s2{3};
    set<int> result;
    setUnion(s1,s2, result);
    REQUIRE(result == set<int>{1,2,3});
}

TEST_CASE( "Set union 2", "[set]" ) {
    set<int> s1{1,2};
    set<int> s2{};
    set<int> result;
    setUnion(s1,s2, result);
    REQUIRE(result == set<int>{1,2});
}

TEST_CASE( "Set union 3", "[set]" ) {
    set<int> s1{1,2};
    set<int> s2{1,2};
    set<int> result;
    setUnion(s1,s2, result);
    REQUIRE(result == set<int>{1,2});
}

TEST_CASE( "Set intersection 1", "[set]" ) {
    set<int> s1{1,2};
    set<int> s2{1};
    set<int> result;
    setIntersection(s1,s2, result);
    REQUIRE(result == set<int>{1});
}

TEST_CASE( "Set intersection 2", "[set]" ) {
    set<int> s1{1,2};
    set<int> s2{};
    set<int> result;
    setIntersection(s1,s2, result);
    REQUIRE(result == set<int>{});
}

TEST_CASE( "Set intersection 3", "[set]" ) {
    set<int> s1{1,2};
    set<int> s2{1,2,3};
    set<int> result;
    setIntersection(s1,s2, result);
    REQUIRE(result == set<int>{1,2});
}

TEST_CASE( "Set inclusion 1", "[set]" ) {
    set<int> s1{1,2};
    set<int> s2{1,2,3};
    bool i = setInclusion(s1,s2);
    REQUIRE(i == true);
}

TEST_CASE( "Set inclusion 2", "[set]" ) {
    set<int> s1{1,2};
    set<int> s2{1,2,3};
    bool i = setInclusion(s2,s1);
    REQUIRE(i == false);
}

TEST_CASE( "Set inclusion 3", "[set]" ) {
    set<int> s1{1,2};
    set<int> s2{1,2};
    bool i = setInclusion(s1,s2);
    REQUIRE(i == true);
}

TEST_CASE( "Set inclusion 4", "[set]" ) {
    set<int> s1{};
    set<int> s2{1,2};
    bool i = setInclusion(s1,s2);
    REQUIRE(i == true);
}

TEST_CASE("Anti chain max 1", "[antiChain]") {
    set<set<int>> s1{{1,2}, {1}, {2}};
    set<set<int>> result;
    antiChainMax(s1, result);
    REQUIRE(result == set<set<int>>{{1,2}});
}

TEST_CASE("Anti chain max 2", "[antiChain]") {
    set<set<int>> s1{{1,2}, {1}, {2}, {3}};
    set<set<int>> result;
    antiChainMax(s1, result);
    REQUIRE(result == set<set<int>>{{1,2}, {3}});
}

TEST_CASE("Anti chain max 3", "[antiChain]") {
    set<set<int>> s1{{1,2}, {1}, {2}, {3}, {1,2,3}};
    set<set<int>> result;
    antiChainMax(s1, result);
    REQUIRE(result == set<set<int>>{{1,2,3}});
}

TEST_CASE("Anti chain max 4", "[antiChain]") {
    set<set<int>> s1{{1},{2},{3}};
    set<set<int>> result;
    antiChainMax(s1, result);
    REQUIRE(result == set<set<int>>{{1},{2},{3}});
}

TEST_CASE("Anti chain max 5", "[antiChain]") {
    set<set<int>> s1{{},{},{}};
    set<set<int>> result;
    antiChainMax(s1, result);
    REQUIRE(result == set<set<int>>{{}});
}

TEST_CASE("Anti chain max 6", "[antiChain]") {
    set<set<int>> s1{{1},{1},{1}};
    set<set<int>> result;
    antiChainMax(s1, result);
    REQUIRE(result == set<set<int>>{{1}});
}

TEST_CASE("Anti chain max 7", "[antiChain]") {
    set<set<int>> s1{{},{1},{1,2},{1}};
    set<set<int>> result;
    antiChainMax(s1, result);
    REQUIRE(result == set<set<int>>{{1,2}});
}

TEST_CASE("Anti chain join 1", "[antiChain]") {
    set<set<int>> s1{{1}, {2,3}, {4,5}};
    set<set<int>> s2{{1,2}, {7}, {8,9}};
    set<set<int>> result;
    antiChainJoin(s1,s2,result);
    REQUIRE(result == set<set<int>>{{1,2}, {2,3}, {4,5}, {7}, {8,9}});
}

TEST_CASE("Anti chain join 2", "[antiChain]") {
    set<set<int>> s1{{1}, {2}, {7}};
    set<set<int>> s2{{1,2}, {7}};
    set<set<int>> result;
    antiChainJoin(s1,s2,result);
    REQUIRE(result == set<set<int>>{{1,2}, {7}});
}

TEST_CASE("Anti chain join 3", "[antiChain]") {
    set<set<int>> s1{{1,2}, {7}};
    set<set<int>> s2{{1,2}, {7}};
    set<set<int>> result;
    antiChainJoin(s1,s2,result);
    REQUIRE(result == set<set<int>>{{1,2}, {7}});
}

TEST_CASE("Anti chain meet 1", "[antiChain]") {
    set<set<int>> s1{{1,2}, {7}};
    set<set<int>> s2{{1,2}, {7}};
    set<set<int>> result;
    antiChainMeet(s1,s2,result);
    REQUIRE(result == set<set<int>>{{1,2}, {7}});
}

TEST_CASE("Anti chain meet 2", "[antiChain]") {
    set<set<int>> s1{{1,2,3}, {7}};
    set<set<int>> s2{{1,2}, {7}};
    set<set<int>> result;
    antiChainMeet(s1,s2,result);
    REQUIRE(result == set<set<int>>{{1,2}, {7}});
}

TEST_CASE("Anti chain meet 3", "[antiChain]") {
    set<set<int>> s1{{1,2,3}, {7,9}};
    set<set<int>> s2{{1,2}, {7,8}};
    set<set<int>> result;
    antiChainMeet(s1,s2,result);
    REQUIRE(result == set<set<int>>{{1,2}, {7}});
}

TEST_CASE("Anti chain meet 4", "[antiChain]") {
    set<set<int>> s1{{}, {7,9}};
    set<set<int>> s2{{1,2}, {7,8}};
    set<set<int>> result;
    antiChainMeet(s1,s2,result);
    REQUIRE(result == set<set<int>>{{7}});
}