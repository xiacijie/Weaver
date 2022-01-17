#pragma once
#include <string>
#include "Log.h"

using namespace std; 
using namespace util;

namespace weaver {

    const string helpMessage = "Usage: ./weaver -f <filename> "
        "-v <lta/normal> "
        "-a <true/false> "
        "-I <smtinterpol/mathsat> "
        "-i <smtinterpol/mathsat/yices> "
        "-h <smtinterpol/mathsat/yices> "
        "-l <none/error/warn/info/debug/verbose> ";

    enum VerifierType { 
        lta, normal
    };

    enum SMTSolverType {
        smtinterpol, mathsat, yices
    };


    class Config {
        public: 
            Config();
            Config(int, char *[]); // command line options 
            string toString();
            VerifierType verifier;
            LogLevel logLevel; 
            string fileName; 
            SMTSolverType interpolantSMTSolver;
            SMTSolverType independenceSMTSolver;
            SMTSolverType hoareTripleSMTSolver;
            bool antiChain;
            
        private: 
            string smtSolverTypeToString(SMTSolverType type);
            void setDefaults(); 

    };
}
