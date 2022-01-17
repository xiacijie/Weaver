#include <string> 
#include <iostream>
#include <regex>
#include "Log.h"

using namespace util;
Logger logger;

/*
   When the current log level value is x,
   messages with level <= x will be displayed.
*/
map<LogLevel, int> levelValues = {{none, 0}, {error, 1},
                                  {warn, 2}, {info, 3}, {debug, 4}, {verbose, 5}};

map<Color, int> colorValuesText = {{magenta, 35}, {blue, 34},
                                   {red, 31}, {yellow , 33}, {green, 32},
                                   {black, 30}, {white, 37}, {cyan, 36}};

map<Color, int> colorValuesBackground = {{magenta, 45}, {blue, 44},
                                         {red, 41}, {yellow , 43}, {green, 42},
                                         {black, 40}, {white, 47}, {cyan, 46}};
/*
The level descriptions each have the same # of
characters, for aesthetic purposes.
*/
map<LogLevel, string> levelDescription = {
        {error,       "  Error  "},
        {warn,        "  Warn   "},
        {info,        "  Info   "},
        {debug,       "  Debug  "},
        {verbose ,    " Verbose "}};

string Logger::toANSI(const string& m, Color tc, Color bc) {
    if (ANSI) {
        return "\x1B[" + to_string(colorValuesText[tc])
        + ";" + to_string(colorValuesBackground[bc]) +
            ";1m" + m + "\x1B[0m";
    } else return m;
}

Logger::Logger() {
    level = LogLevel::info;
    ANSI = true;
}

Logger::Logger(LogLevel l) {
    level = l;
    ANSI = true;
}

void Logger::setLogLevel(LogLevel l) {
    level = l;
}

void Logger::error(const string& m) {
    print(LogLevel::error, white, red, m);
}

void Logger::warn(const string& m) {
    print(LogLevel::warn, black, yellow, m);
}

void Logger::info(const string& m) {
    print(LogLevel::info, white, blue, m);
}

void Logger::debug(const string& m) {
    print(LogLevel::debug, black, white, m);
}

void Logger::verbose(const string& m) {
    print(LogLevel::verbose, white, green, m);
}

void Logger::print(LogLevel l, Color tc, Color bc, const string& msg) {
    print(l,toANSI(levelDescription[l], tc, bc) + " " +
    indent(msg));
}

void Logger::print(LogLevel l, const string& m) {
    if (checkLevel(l)) {
        cout << m << endl;
    }
}

bool Logger::checkLevel(LogLevel l) {
    if (l == LogLevel::none)
        return false;

    return levelValues[l] <= levelValues[level];
}

string Logger::indent(const string& m) {
    return regex_replace(m, std::regex("\n"), "\n          ");
}

