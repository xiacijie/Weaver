#pragma once
#include <string>
#include "Log.h"

using namespace std; 
using namespace util;

namespace weaver {

    const string helpMessage = "Usage: ./weaver -f <filename> "
        "-v <lta/normal> "
        "-a <true/false>"
        "-l <none/error/warn/info/debug/verbose>";

    enum Settings {
        verifier, loglevel
    }; 

    enum VerifierType { 
        lta, normal
    };

    

    class Config {
        public: 
            Config();
            Config(int, char *[]); // command line options 
            string toString();
            VerifierType verifier;
            LogLevel logLevel; 
            string fileName; 
            bool antiChain;
            
        private: 
            void setDefaults(); 

    };
}
