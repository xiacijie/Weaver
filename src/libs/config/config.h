#include <string> 
#include "log.h"

using namespace std; 
using namespace logg; 

namespace conf {

    const string helpMessage = "Usage: ./weaver -f <filename> " 
        "-v <parallel/sequential> "
        "-l <none/error/warn/info/debug/verbose>";

    enum Settings {
        verifier, loglevel
    }; 

    enum VerifierType { 
        parallel, sequential
    };

    class Config {
        public: 
            Config();
            Config(int, char *[]); // command line options 
            string toString();
            VerifierType verifier;
            LogLevel logLevel; 
            string fileName; 
        private: 
            void setDefaults(); 

    };

}