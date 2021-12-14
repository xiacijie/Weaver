#include <iostream>
#include <string>
#include <catch2/catch_test_macros.hpp>
#include "antlr4-runtime.h"
#include "WeaverLexer.h"
#include "WeaverParser.h"
#include "ASTBuilder.h"
#include "CFGBuilder.h"
#include "Program.h"
#include "SequentialProgramVerifier.h"
#include "LoopingTreeAutomata.h"
#include "ParallelProgramVerifier.h"

using namespace weaver;
using namespace std;
using namespace antlr4;
using namespace antlrcpp;

unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( Factorial(1) == 1 );
    REQUIRE( Factorial(2) == 2 );
    REQUIRE( Factorial(3) == 6 );
    REQUIRE( Factorial(10) == 3628800 );
}