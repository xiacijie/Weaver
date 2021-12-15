#include <unistd.h>
#include <string>
#include "config.h" 

conf::Config config;  
extern logg::Logger logger; 

using namespace std; 

namespace conf {
    Config::Config() {
        this->setDefaults(); 
    }
    Config::Config(int argc, char *argv[]) {

        int opt;
        string parameter; // To coerce optarg to a string
        this->setDefaults(); 
        
        // Parse command line options: 
        while (opt = getopt(argc, argv, "f:v:l:")) {
            switch(opt) {
                case 'f':
                    parameter = optarg; 
                    this->fileName = parameter; 
                    continue; 
                case 'v':
                    parameter = optarg; 
                    if (parameter == "parallel") {
                        this->verifier = conf::parallel; 
                    } else if (parameter == "sequential") {
                        this->verifier = conf::sequential; 
                    }
                    continue; 
                case 'l':
                    parameter = optarg; 
                    if (parameter == "none") {
                        this->logLevel = logg::none; 
                    } else if (parameter == "error") {
                        this->logLevel = logg::error; 
                    } else if (parameter == "warn") {
                        this->logLevel = logg::warn; 
                    } else if (parameter == "info") {
                        this->logLevel = logg::info; 
                    } else if (parameter == "debug") {
                        this->logLevel = logg::debug; 
                    } else if (parameter == "verbose") {
                        this->logLevel = logg::verbose; 
                    } else {
                        logger.error("Unknown log level.\n"
                            + conf::helpMessage);
                        exit(1);
                    }
                    continue; 
                default: 
                    logger.error(conf::helpMessage);
                    exit(1); 
                    break; 
                case -1:
                    break;
            }
            break; 
        }

    }

    string Config::toString() {
        string verif;
        string logLevel; 

        switch(this->verifier) {
            case parallel:
                verif = "parallel";
                break; 
            case sequential:
                verif = "sequential";
                break; 
            default: 
                verif = ""; 
        }

        switch(this->logLevel) {
            case logg::none: 
                logLevel = "none"; 
                break; 
            case logg::error:
                logLevel = "error"; 
                break; 
            case logg::warn:
                logLevel = "warn"; 
                break; 
            case logg::info:
                logLevel = "info"; 
                break; 
            case logg::debug:
                logLevel = "debug"; 
                break; 
            case logg::verbose: 
                logLevel = "verbose"; 
                break; 
            default: 
                logLevel = ""; 
        }

        return "VERIFIER:       " + verif +
             "\nLOG LEVEL:      " + logLevel + 
             "\nINPUT FILE:     " + this->fileName; 
    }

    void Config::setDefaults() {
        // Set defaults: 
        this->verifier = conf::sequential;
        this->logLevel = logg::info; 
        this->fileName = "";
    }
}