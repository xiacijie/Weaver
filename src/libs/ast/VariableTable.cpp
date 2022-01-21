#include <iostream>
#include <algorithm>
#include <sstream>
#include "VariableTable.h"

using namespace std;
using namespace weaver;

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

DataType VariableTable::getVarType(const string &varName) {
    return _varTable[varName];
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