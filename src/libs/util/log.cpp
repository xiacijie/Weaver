#include <string> 
#include <iostream>
#include <regex>
#include "log.h"

logg::Logger logger;

namespace logg {
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

    string Logger::toANSI(string m, Color tc, Color bc) {
        if (this->ANSI) {
            return "\x1B[" + to_string(colorValuesText[tc]) 
            + ";" + to_string(colorValuesBackground[bc]) + 
                ";1m" + m + "\x1B[0m";
        } else return m; 
    }

    Logger::Logger() {
        this->level = logg::info;
        this->ANSI = true; 
    }

    Logger::Logger(LogLevel l) {
        this->level = l;
        this->ANSI = true; 
    }

    void Logger::setLogLevel(LogLevel l) {
        this->level = l;
    }

    void Logger::error(string m) {
        this->print(logg::error, logg::white, logg::red, m);
    }

    void Logger::warn(string m) {
        this->print(logg::warn, logg::black, logg::yellow, m);
    }

    void Logger::info(string m) {
        this->print(logg::info, logg::white, logg::blue, m);
    }

    void Logger::debug(string m) {
        this->print(logg::debug, logg::black, logg::white, m);
    }

    void Logger::verbose(string m) {
        this->print(logg::verbose, logg::white, logg::green, m);
    } 

    void Logger::print(LogLevel l, Color tc, Color bc, string msg) {
        this->print(l,toANSI(levelDescription[l], tc, bc) + " " + 
            this->indent(msg));
    }

    void Logger::print(LogLevel l, string m) {
        if (this->checkLevel(l)) {
            cout << m << endl;
        }
    }

    bool Logger::checkLevel(LogLevel l) {
        if (l == logg::none) return false; 
        return levelValues[l] <= levelValues[this->level];   
    }

    string Logger::indent(string m) {
        return regex_replace(m, std::regex("\n"), "\n          ");
    }

}