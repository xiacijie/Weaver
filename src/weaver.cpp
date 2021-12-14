#include <iostream>
#include <string>
#include "antlr4-runtime.h"
#include "libs/frontend/WeaverLexer.h"
#include "libs/frontend/WeaverParser.h"
#include "libs/ast/ASTBuilder.h"
#include "libs/cfg/CFGBuilder.h"
#include "libs/program/Program.h"
#include "libs/verifier/SequentialProgramVerifier.h"
#include "libs/automata/LoopingTreeAutomata.h"
#include "libs/verifier/ParallelProgramVerifier.h"


using namespace weaver;
using namespace std;
using namespace antlr4;
using namespace antlrcpp;


int main(int argc , const char ** argv) {
	
	if (argc < 2) {
		cerr << "weaver: fatal error: no input files. Try with './weaver <file>.wvr'" << endl;
		return 1;
	}

    Program program = Program();
    program.init(argv[1]);

    ParallelProgramVerifier c(&program);
    c.verify();
//    SequentialProgramVerifier v(&program);
//    v.verify();
	return 0;
}