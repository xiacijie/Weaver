#pragma once 
#include <string> 
#include <vector>
#include <map>

using namespace std;

namespace util {
    enum LogLevel { none, error, warn, info, debug, verbose };
    enum Color { magenta, blue, yellow, red, green, black, white, cyan };

    class Logger {
        public:
            Logger();
            Logger(LogLevel);
            void setLogLevel(LogLevel);
            void error(const string&);
            void warn(const string&);
            void info(const string&);
            void debug(const string&);
            void verbose(const string&);
        private:
            void print(LogLevel, const string&);
            void print(LogLevel, Color, Color, const string&);
            bool checkLevel(LogLevel);
            string toANSI(const string&, Color, Color);
            string indent(const string&);
            LogLevel level;
            bool ANSI; 
    };
}
