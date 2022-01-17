#include "SSANumberingTable.h"
#include <cassert>

using namespace weaver;
using namespace std;

void SSANumberingTable::incNumber(const string& varName) {
    if (!hasVar(varName)) {
        _numberTable[varName] = 0;
    }
    else {
        _numberTable[varName]++;
    }
}


bool SSANumberingTable::hasVar(const string& varName) {
    return _numberTable.find(varName) != _numberTable.end();
}

uint16_t SSANumberingTable::getNumber(const string& varName) {
    assert(hasVar(varName) && "Error");
    return _numberTable[varName];
}


void SSANumberingTable::declareDefinedVar(const string& varName) {
    incNumber(varName);
    _varsDefinedToBeDeclared.push(make_pair(varName, getNumber(varName)));
}

void SSANumberingTable::declareUndefinedVar(const string& varName) {
    incNumber(varName);
    _varsUndefinedToBeDeclared.push(make_pair(varName, getNumber(varName)));
}

pair<string, uint16_t> SSANumberingTable::popVarToDeclare() {
    assert(hasVarsToDeclare() && "Error!");
    if (hasDefinedVarsToDeclare()) {
        return popDefinedVarToDeclare();
    }

    if (hasUndefinedVarsToDeclare()) {
        return popUndefinedVarToDeclare();
    }
}

pair<string, uint16_t> SSANumberingTable::popDefinedVarToDeclare() {
    assert(hasDefinedVarsToDeclare() && "Error!");

    auto p = _varsDefinedToBeDeclared.front();
    _varsDefinedToBeDeclared.pop();

    return p;
}

pair<string, uint16_t> SSANumberingTable::popUndefinedVarToDeclare() {
    assert(hasUndefinedVarsToDeclare() && "Error!");

    auto p = _varsUndefinedToBeDeclared.front();
    _varsUndefinedToBeDeclared.pop();

    return p;
}
