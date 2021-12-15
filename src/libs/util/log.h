#pragma once 
#include <string> 
#include <vector>
#include <map>

using namespace std; 

namespace logg {

    enum LogLevel { none, error, warn, info, debug, verbose };
    enum Color { magenta, blue, yellow, red, green, black, white, cyan };

    class Logger {
        public:
            Logger(); 
            Logger(LogLevel); 
            void setLogLevel(LogLevel);
            void error(string); 
            void warn(string); 
            void info(string); 
            void debug(string);
            void verbose(string); 
        private:
            void print(LogLevel, string);
            void print(LogLevel, Color, Color, string);
            bool checkLevel(LogLevel);
            string toANSI(string, Color, Color); 
            string indent(string);
            LogLevel level;
            bool ANSI; 
    };
}

extern logg::Logger logger; 