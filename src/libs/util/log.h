#pragma once 
#include <string> 
#include <vector>
#include <map>

using namespace std; 

namespace logg {

    enum LogLevel { none, error, info, debug, verbose };
    enum Color { magenta, blue, yellow, red, green };

    class Logger {
        public:
            Logger(); 
            Logger(LogLevel); 
            void setLogLevel(LogLevel);
            void error(string); 
            void info(string); 
            void debug(string);
            void verbose(string); 
        private:
            void print(LogLevel, string);
            bool checkLevel(LogLevel);
            string toANSI(string, Color); 
            LogLevel level;
            bool ANSI; 
    };
}

extern logg::Logger logger; 