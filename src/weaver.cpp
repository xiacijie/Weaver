#include <iostream>
#include <string>
#include "libs/program/Program.h"
#include "libs/verifier/SequentialProgramVerifier.h"
#include "libs/verifier/ParallelProgramVerifier.h"
#include "log.h"
#include "config.h"

using namespace weaver;
using namespace std;

extern logg::Logger logger;
extern conf::Config config; 

int main(int argc , char *argv[]) {

    config = conf::Config(argc, argv); 
    logger = logg::Logger(config.logLevel); 

    if (config.fileName == "") {
        logger.error(conf::helpMessage); 
        exit(1);
    }

    logger.info("Running Weaver with the following settings:\n" + 
        config.toString());

    // You can remove these. This is just to demonstrate
    // the usage of the logger. 
    logger.error("An error."); // Can't continue
    logger.warn("An warning."); // This might be a problem
    logger.info("Info."); // Where are we at? 
    logger.debug("Debugging"); // Look under the hood
    logger.verbose("verbose"); // I want all information

    Program program = Program();
    program.init(config.fileName);

    if (config.verifier == conf::parallel) {
        ParallelProgramVerifier c(&program);
        c.verify();
    } else if (config.verifier == conf::sequential) {
        SequentialProgramVerifier v(&program);
        v.verify();
    }

	return 0;
}