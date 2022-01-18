# Weaver
Automated Hypersafety Verification in C++

This project uses `SMTInterpol`, `MathSAT` and `Yices` as the SMT solvers.

The binaries for those three SMT solvers are provided under the `src/bin` directory so you don't need to install them separately. 

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
7. `./weaver -f ../example/test.wvr`

## Argument options:
1. `-f` should take the input file to verify. This must be specified.
2. `-v` takes either `normal` or `lta`, which means the verify algorithm will use determinstic finite automata or looping tree automata to verify the program.
3. `-a` takes either `true` or `false`, which means whether to use the antichain algorithms for the looping tree automata proof checking.
4. `-I` takes `<smtinterpol/mathsat>`. This specifies the SMT solver used for craig interpolation.
5. `-i` takes `<smtinterpol/mathsat/yices>`. This specifies the SMT solver used for checking independence relations of statements.
6. `-h` takes `<smtinterpol/mathsat/yices>`. This specifies the SMT solver used for checking the validity of hoare triples during proof construction.
7. `-l` takes `none/error/warn/info/debug/verbose` 

## Default options:

If no options other than `-f` is specified, Weaver uses `lta` and the antichain algorithm for proof checking. `mathsat` for craig interpolations and independence relation checking. `yices` for checking validity of hoare tripes. `info` for the log level. 

## Note:
This project uses ANTLR4 as the front end parsing framework. If any error regarding ANTLR4 happens, visit https://www.antlr.org for solution. 
