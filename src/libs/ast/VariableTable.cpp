#include <iostream>
#include <algorithm>
#include <sstream>
#include "VariableTable.h"
#include "log.h"

using namespace std;
using namespace weaver;

extern logg::Logger logger;

void VariableTable::initializeVar(const string &varName) {
    _varInitializationTable[varName] = true;
}

void VariableTable::declareVar(const string &varName, DataType type) {
    _varTable[varName] = type;
    _varInitializationTable[varName] = false;
}

bool VariableTable::isVarInitialized(const string &varName) {
    return _varInitializationTable[varName];
}

bool VariableTable::anyUninitializedVar() {
    return any_of(_varInitializationTable.begin(), _varInitializationTable.end(), [=](const auto& it) {
       return it.second == false;
    });
}

DataType VariableTable::getVarType(const string &varName) {
    return _varTable[varName];
}

unordered_map<string, DataType>& VariableTable::getGlobalVariables() {
    return _varTable;
}

bool VariableTable::isVarDeclared(const string &varName) {
    return _varTable.find(varName) != _varTable.end();
}

string VariableTable::toString() {
    stringstream ss;
    ss << "VARIABLE TABLE:" << endl;

    for (auto& it: _varTable) {
        ss << it.first + " : " + DataTypeLabels[it.second] + "\n";
    }

    return ss.str();
}