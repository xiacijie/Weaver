# minimum required CMAKE version
CMAKE_MINIMUM_REQUIRED(VERSION 3.7 FATAL_ERROR)

project(weaver)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# compiler must be 17
set(CMAKE_CXX_STANDARD 17)

# required if linking to static library
add_definitions(-DANTLR4CPP_STATIC)

# using /MD flag for antlr4_runtime (for Visual C++ compilers only)
set(ANTLR4_WITH_STATIC_CRT OFF)
# add external build for antlrcpp
include(ExternalAntlr4Cpp)
# add antrl4cpp artifacts to project environment
include_directories(${ANTLR4_INCLUDE_DIRS} )

# set variable pointing to the antlr tool that supports C++
# this is not required if the jar file can be found under PATH environment
set(ANTLR_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/bin/antlr4/antlr-4.9.2-complete.jar)
# add macros to generate ANTLR Cpp code from grammar
find_package(ANTLR REQUIRED)
find_package(Threads)

# Call macro to add lexer and grammar to your build dependencies.
antlr_target(WeaverGrammarLexer ${CMAKE_CURRENT_SOURCE_DIR}/grammar/WeaverLexer.g4
            LEXER VISIOR OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/libs/frontend
            PACKAGE weaver weaver_program)
antlr_target(WeaverGrammarParser ${CMAKE_CURRENT_SOURCE_DIR}/grammar/WeaverParser.g4
            PARSER VISITOR OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/libs/frontend
            PACKAGE weaver weaver_program
            DEPENDS_ANTLR WeaverGrammarLexer
            COMPILE_FLAGS -lib ${ANTLR_WeaverGrammarLexer_OUTPUT_DIR})

# include generated files in project environment
include_directories(${ANTLR_WeaverGrammarLexer_OUTPUT_DIR})
include_directories(${ANTLR_WeaverGrammarParser_OUTPUT_DIR})
include_directories(${PROJECT_SOURCE_DIR}/libs/program)
include_directories(${PROJECT_SOURCE_DIR}/libs/cfg)
include_directories(${PROJECT_SOURCE_DIR}/libs/ast)
include_directories(${PROJECT_SOURCE_DIR}/libs/automata)
include_directories(${PROJECT_SOURCE_DIR}/libs/verifier)
include_directories(${PROJECT_SOURCE_DIR}/libs/util)

add_library(weaver_antlr 
       ${ANTLR_WeaverGrammarLexer_CXX_OUTPUTS}
       ${ANTLR_WeaverGrammarParser_CXX_OUTPUTS})

add_library(weaver_program 
       ${PROJECT_SOURCE_DIR}/libs/program/Program.cpp
       ${PROJECT_SOURCE_DIR}/libs/program/Statement.cpp
       ${PROJECT_SOURCE_DIR}/libs/program/Thread.cpp)

target_link_libraries(weaver_program antlr4_static weaver_antlr utils)

add_library(ast 
       ${PROJECT_SOURCE_DIR}/libs/ast/AST.cpp
       ${PROJECT_SOURCE_DIR}/libs/ast/ASTNode.cpp
       ${PROJECT_SOURCE_DIR}/libs/ast/VariableTable.cpp
       ${PROJECT_SOURCE_DIR}/libs/ast/ASTBuilder.cpp)

add_library(cfg ${PROJECT_SOURCE_DIR}/libs/cfg/CFGBuilder.cpp)

add_library(automata  
       ${PROJECT_SOURCE_DIR}/libs/automata/FiniteAutomataBase.cpp
       ${PROJECT_SOURCE_DIR}/libs/automata/NFA.cpp
       ${PROJECT_SOURCE_DIR}/libs/automata/DFA.cpp
       ${PROJECT_SOURCE_DIR}/libs/automata/LoopingTreeAutomata.cpp)

add_library(verifier 
       ${PROJECT_SOURCE_DIR}/libs/verifier/InterpolantAutomataBuilder.cpp
       ${PROJECT_SOURCE_DIR}/libs/verifier/TheoremProverBase.cpp
       ${PROJECT_SOURCE_DIR}/libs/verifier/SMTInterpol.cpp
       ${PROJECT_SOURCE_DIR}/libs/verifier/SequentialProgramVerifier.cpp
       ${PROJECT_SOURCE_DIR}/libs/verifier/ParallelProgramVerifier.cpp)

add_library(utils ${PROJECT_SOURCE_DIR}/libs/util/util.cpp
${PROJECT_SOURCE_DIR}/libs/util/log.cpp)

# add generated grammar to weaver binary target
add_executable(${PROJECT_NAME} weaver.cpp)
# the global path for theorem prover binaries
add_definitions(-DSMT_INTERPOL_BIN_PATH=\"${PROJECT_SOURCE_DIR}/bin/smt-interpol/smt-interpol.jar\")

target_link_libraries(weaver 
       antlr4_static 
       weaver_antlr 
       utils 
       automata 
       weaver_program
       ast cfg verifier
       )
# target_link_libraries(weaver ${CMAKE_THREAD_LIBS_INIT})

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.0-preview3
)

FetchContent_MakeAvailable(Catch2)

include_directories(${PROJECT_SOURCE_DIR}/libs/frontend)

add_executable(tests 
       test/test.cpp)
target_link_libraries(tests PRIVATE 
       Catch2::Catch2WithMain
       antlr4_static 
       weaver_antlr 
       utils 
       automata 
       weaver_program
       ast cfg verifier
       )

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(tests [WORKING_DIRECTORY .])

