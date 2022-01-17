#include <unistd.h>
#include <string>
#include "Config.h"

using namespace std; 
using namespace weaver;

extern Logger logger;
Config config;

Config::Config() {
    setDefaults();
}

Config::Config(int argc, char *argv[]) {

    int opt;
    string parameter; // To coerce optarg to a string
    setDefaults();

    // Parse command line options:
    while (opt = getopt(argc, argv, "f:v:a:l:")) {
        switch(opt) {
            case 'f':
                parameter = optarg;
                fileName = parameter;
                continue;
            case 'v':
                parameter = optarg;
                if (parameter == "lta") {
                    verifier = VerifierType::lta;
                } else if (parameter == "normal") {
                    verifier = VerifierType::normal;
                }
                continue;
            case 'l':
                parameter = optarg;
                if (parameter == "none") {
                    logLevel = LogLevel::none;
                } else if (parameter == "error") {
                    logLevel = LogLevel::error;
                } else if (parameter == "warn") {
                    logLevel = LogLevel::warn;
                } else if (parameter == "info") {
                    logLevel = LogLevel::info;
                } else if (parameter == "debug") {
                    logLevel = LogLevel::debug;
                } else if (parameter == "verbose") {
                    logLevel = LogLevel::verbose;
                } else {
                    logger.error("Unknown log level.\n"
                        + helpMessage);
                    exit(1);
                }
                continue;
            case 'a':
                if (verifier != VerifierType::lta) {
                    logger.error("Only LTA can use antiChain option!");
                    exit(1);
                }
              
                parameter = optarg;
                if (parameter == "true") {
                    antiChain = true;
                }
                else {
                    antiChain = false;
                }
                continue;
                
            default:
                logger.error(helpMessage);
                exit(1);
                break;
            case -1:
                break;
        }
        break;
    }

}

string Config::toString() {
    string v;
    string l;

    switch(verifier) {
        case VerifierType::lta:
            v = "lta";
            break; 
        case VerifierType::normal:
            v = "normal";
            break; 
        default:
            v = "";
    }

    switch(logLevel) {
        case LogLevel::none:
            l = "none";
            break; 
        case LogLevel::error:
            l = "error";
            break; 
        case LogLevel::warn:
            l = "warn";
            break; 
        case LogLevel::info:
            l = "info";
            break; 
        case LogLevel::debug:
            l = "debug";
            break; 
        case LogLevel::verbose:
            l = "verbose";
            break; 
        default:
            l = "";
    }

    return "VERIFIER:       " + v +
           "\nLOG LEVEL:      " + l +
           "\nINPUT FILE:     " + fileName;
}

void Config::setDefaults() {
    // Set defaults:
    verifier = VerifierType::normal;
    logLevel = LogLevel::info;
    fileName = "";
}
