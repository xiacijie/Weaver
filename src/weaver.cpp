#include <string>
#include "Program.h"
#include "LoopingTreeAutomataVerifier.h"
#include "FiniteAutomataVerifier.h"
#include "Log.h"
#include "Config.h"

using namespace weaver;
using namespace util;
using namespace std;

extern Config config;
extern Logger logger;

int main(int argc , char *argv[]) {
    config = Config(argc, argv);
    logger = Logger(config.logLevel);

    if (config.fileName.empty()) {
        logger.error(helpMessage);
        exit(1);
    }

    logger.info("Running Weaver with the following settings:\n" +
                                config.toString());

    Program program = Program();
    program.init(config.fileName);

    if (config.verifier == VerifierType::lta) {
        LoopingTreeAutomataVerifier v(&program);
        v.verify();
    }
    else {
        FiniteAutomataVerifier v(&program);
        v.verify();
    }
    

	return 0;
}