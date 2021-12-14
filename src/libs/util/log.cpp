#include <string> 
#include <iostream>
#include "log.h"

namespace logg {

    map<logg::LogLevel, int> levelValues = {{logg::none, 0}, {logg::error, 1}, 
        {logg::info, 2}, {logg::debug, 3}, {logg::verbose, 4}};
    
    map<logg::Color, int> colorValues = {{logg::magenta, 35}, {logg::blue, 34}, 
        {logg::red, 31}, {logg::yellow , 33}, {logg::green, 32}};

    string Logger::toANSI(string m, Color c) {
        if (this->ANSI) {
            return "\x1B[0;" + to_string(colorValues[c]) + 
                "m" + m + "\x1B[0m";
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

        this->print(logg::error, 
            toANSI("Error", logg::red) + ": " + m);
    }

    void Logger::info(string m) {
        this->print(logg::info, 
            toANSI("Info", logg::blue) + ": " + m);
    }

    void Logger::debug(string m) {
        this->print(logg::debug, 
            toANSI("Debug", logg::yellow) + ": " + m);
    }

    void Logger::verbose(string m) {
        this->print(logg::verbose, 
            toANSI("Verbose", logg::green) + ": " + m);
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

}

