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
7. `./weaver -f ../example/test.wvr -v lta -a true -l none`

## Argument options:
1. `-f` should take the input file to verify
2. `-v` takes either `normal` or `lta`, which means the verify algorithm will use determinstic finite automata or looping tree automata to verify the program.
3. `-a` takes either `true` or `false`, which means whether to use the antichain algorithms for the looping tree automata proof checking.
4. `-l` takes `none/error/warn/info/debug/verbose` 


## Note:
This project uses ANTLR4 as the front end parsing framework. If any error regarding ANTLR4 happens, visit https://www.antlr.org for solution. 
