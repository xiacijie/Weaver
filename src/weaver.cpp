#include <string>
#include "Program.h"
#include "ParallelProgramVerifier.h"
#include "SequentialProgramVerifier.h"
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

    SequentialProgramVerifier c(&program);
    c.verify();

	return 0;
}