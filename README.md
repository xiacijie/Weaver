# Weaver
Automated Hypersafety Verification in C++

This project uses SMTInterpol, MathSAT and Yices as the SMT solvers. 

## Environment Requirements:
1. Java 11
2. C++ 14
3. CMake >= 3.7

## Build Instructions:
1. Clone this project.
2. `cd Weaver`
3. `mkdir build`
4. `cd build`
5. `cmake ../src`
6. `make`
7. `./weaver ../example/test.wvr`

## Note:
This project uses ANTLR4 as the front end parsing framework. If any error regarding ANTLR4 happens, visit https://www.antlr.org for solution. 
