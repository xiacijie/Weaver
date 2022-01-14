#include "MathSAT.h"
#include "Program.h"
#include "ASTNode.h"
#include <catch2/catch_test_macros.hpp>

using namespace std;
using namespace weaver;

TEST_CASE( "Test SMT independence checking", "[smt]" ) {

    Program p;
    p.getVariableTable().declareVar("a", DataType::Int);
    p.getVariableTable().declareVar("b", DataType::Int);

    auto const1 = ASTNode::create(1, &p);
    auto const2 = ASTNode::create(2, &p);

    auto id_a = ASTNode::create("a", DataType::Int, &p);
    auto id_b = ASTNode::create("b", DataType::Int, &p);

    auto a_plus_1 = ASTNode::create(NodeType::Addition, DataType::Int, &p, {id_a, const1});
    auto a_minus_1 = ASTNode::create(NodeType::Subtraction, DataType::Int, &p, {id_a, const1});

    auto a_eq_a_plus_1 = ASTNode::create(NodeType::Assignment, DataType::NoType, &p, {id_a, a_plus_1});
    auto a_eq_a_minus_1 = ASTNode::create(NodeType::Assignment, DataType::NoType, &p, {id_a, a_minus_1});

    auto a_eq_1 = ASTNode::create(NodeType::Assignment, DataType::NoType, &p, {id_a, const1});
    auto a_eq_2 = ASTNode::create(NodeType::Assignment, DataType::NoType, &p, {id_a, const2});

    auto b_eq_1 = ASTNode::create(NodeType::Assignment, DataType::NoType, &p, {id_b, const1});

    auto solver = p.getMathSAT();


    SECTION( "a = a + 1; & a = a - 1;" ) {
        auto s1 = Statement::create(a_eq_a_plus_1, &p, 1);
        auto s2 = Statement::create(a_eq_a_minus_1, &p, 2);
        bool ind = solver->checkIndependenceRelation(s1, s2);
        REQUIRE(ind == true);
    }

    SECTION( "a = 1; & a = 2;" ) {
        auto s1 = Statement::create(a_eq_1, &p, 1);
        auto s2 = Statement::create(a_eq_2, &p, 2);
        bool ind = solver->checkIndependenceRelation(s1, s2);
        REQUIRE(ind == false);
    }

    SECTION( "a = a + 1; & b = 1;" ) {
        auto s1 = Statement::create(a_eq_a_plus_1, &p, 1);
        auto s2 = Statement::create(b_eq_1, &p, 2);
        bool ind = solver->checkIndependenceRelation(s1, s2);
        REQUIRE(ind == true);
    }
}