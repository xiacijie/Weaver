# minimum required CMAKE version
CMAKE_MINIMUM_REQUIRED(VERSION 3.7 FATAL_ERROR)

project(weaver)

#set the build type
set(CMAKE_BUILD_TYPE "Debug")

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
            PACKAGE weaver)
antlr_target(WeaverGrammarParser ${CMAKE_CURRENT_SOURCE_DIR}/grammar/WeaverParser.g4
            PARSER VISITOR OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/libs/frontend
            PACKAGE weaver
            DEPENDS_ANTLR WeaverGrammarLexer
            COMPILE_FLAGS -lib ${ANTLR_WeaverGrammarLexer_OUTPUT_DIR})

# the global path for theorem prover binaries
add_definitions(-DSMT_INTERPOL_BIN_PATH=\"${PROJECT_SOURCE_DIR}/bin/smt-interpol/smt-interpol.jar\")
add_definitions(-DMATH_SAT_BIN_PATH=\"${PROJECT_SOURCE_DIR}/bin/math-sat/mathsat\")
add_definitions(-DYICES_BIN_PATH=\"${PROJECT_SOURCE_DIR}/bin/yices/yices-smt2\")

#supress warnings
add_definitions(-w)

# include generated files in project environment
include_directories(${ANTLR_WeaverGrammarLexer_OUTPUT_DIR})
include_directories(${ANTLR_WeaverGrammarParser_OUTPUT_DIR})
include_directories(${PROJECT_SOURCE_DIR}/libs/program)
include_directories(${PROJECT_SOURCE_DIR}/libs/cfg)
include_directories(${PROJECT_SOURCE_DIR}/libs/ast)
include_directories(${PROJECT_SOURCE_DIR}/libs/automata)
include_directories(${PROJECT_SOURCE_DIR}/libs/theoremprover)
include_directories(${PROJECT_SOURCE_DIR}/libs/verifier)
include_directories(${PROJECT_SOURCE_DIR}/libs/util)
include_directories(${PROJECT_SOURCE_DIR}/libs/config)
include_directories(${PROJECT_SOURCE_DIR}/libs/global)

add_subdirectory(${PROJECT_SOURCE_DIR}/libs/ast)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/automata)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/cfg)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/config)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/program)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/util)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/verifier)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/theoremprover)

add_library(frontend
        ${ANTLR_WeaverGrammarLexer_CXX_OUTPUTS}
        ${ANTLR_WeaverGrammarParser_CXX_OUTPUTS})

# add generated grammar to weaver binary target
add_executable(${PROJECT_NAME} weaver.cpp)

target_link_libraries(frontend
        antlr4_static)

target_link_libraries(weaver 
        antlr4_static
        frontend
        util
        automata
        program
        ast
        cfg
        verifier
        theoremprover
        config
        ${CMAKE_THREAD_LIBS_INIT}
       )

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.0-preview3
)

FetchContent_MakeAvailable(Catch2)

add_executable(testUtil
       test/testUtil.cpp)
add_executable(testSMT
       test/testSMT.cpp)

target_link_libraries(testUtil PRIVATE
        Catch2::Catch2WithMain
        util)

target_link_libraries(testSMT PRIVATE
        Catch2::Catch2WithMain
        program
        antlr4_static
        theoremprover
        ast 
        cfg 
        automata
        )

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)


