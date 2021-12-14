#include <iostream>
#include <string>
#include "libs/program/Program.h"
#include "libs/verifier/SequentialProgramVerifier.h"
#include "libs/automata/LoopingTreeAutomata.h"
#include "libs/verifier/ParallelProgramVerifier.h"
#include "log.h"

using namespace weaver;
using namespace std;

logg::Logger logger;

int main(int argc , const char ** argv) {

    logger = logg::Logger(logg::verbose); 
	
	if (argc < 2) {
        logger.error("No input files. \
            Try with './weaver <file>.wvr'"); 
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