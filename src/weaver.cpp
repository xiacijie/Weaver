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
	
	ifstream stream;

	// Need to provide the code file
	if (argc < 2) {
		cerr << "weaver: fatal error: no input files. Try with './weaver <file>.wvr'" << endl;
		return 1;
	}

	string inputFile = argv[1];
    stream.open(inputFile);

	ANTLRInputStream input(stream);
    WeaverLexer lexer(&input);
	CommonTokenStream tokens(&lexer);

	tokens.fill();

	WeaverParser parser(&tokens);


	WeaverParser::ProgramContext* tree = parser.program();

    if (parser.getNumberOfSyntaxErrors() > 0) {
        cerr << "Weaver: Syntax Error!\n" << endl;
        abort();
    }

    //=======================
    Program program;
    ASTBuilder builder(&program, tree);
    builder.build();

    cout << program.getAST().toString();

    CFGBuilder cfgBuilder(&program);
    cout << "Building CFG..." << endl;
    cfgBuilder.build();
    cout << program.getCFG().toString() << endl;

    cout << "Alphabet size: " << program.getAlphabet().size() << endl;

    program.buildDependenceRelation();

    cout << program.independentStatementsToString() << endl;

    cout << program.dependentStatementsToString() << endl;

    ParallelProgramVerifier c(&program);
    c.verify();
//    SequentialProgramVerifier v(&program);
//    v.verify();
	return 0;
}