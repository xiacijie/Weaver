#include <iostream>
#include <string>
#include "libs/program/Program.h"
#include "libs/verifier/SequentialProgramVerifier.h"
#include "libs/automata/LoopingTreeAutomata.h"
#include "libs/verifier/ParallelProgramVerifier.h"
#include "log.h"

using namespace weaver;
using namespace std;

extern logg::Logger logger;

int main(int argc , const char ** argv) {

    logger = logg::Logger(logg::verbose); 
	
	if (argc < 2) {
        logger.error("No input files. \
            Try with './weaver <file>.wvr'"); 
		return 1;
	}

    // You can remove these. This is just to demonstrate
    // the usage of the logger. 
    logger.error("An error."); // Can't continue
    logger.warn("An warning."); // This might be a problem
    logger.info("Info."); // Where are we at? 
    logger.debug("Debugging"); // Look under the hood
    logger.verbose("verbose"); // I want all information

    Program program = Program();
    program.init(argv[1]);

    ParallelProgramVerifier c(&program);
    c.verify();
//    SequentialProgramVerifier v(&program);
//    v.verify();
	return 0;
}