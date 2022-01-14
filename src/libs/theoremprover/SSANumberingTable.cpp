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


void SSANumberingTable::declareInitializedVar(const string& varName) {
    incNumber(varName);
    _varsInitializedToBeDeclared.push(make_pair(varName, getNumber(varName)));
}

void SSANumberingTable::declareUnInitializedVar(const string& varName) {
    incNumber(varName);
    _varsUnIntializedToBeDeclared.push(make_pair(varName, getNumber(varName)));
}

pair<string, uint16_t> SSANumberingTable::popVarToDeclare() {
    assert(hasVarsToDeclare() && "Error!");
    if (hasInitializedVarsToDeclare()) {
        return popInitializedVarToDeclare();
    }

    if (hasUnInitializedVarsToDeclare()) {
        return popUnInitializedVarToDeclare();
    }
}

pair<string, uint16_t> SSANumberingTable::popInitializedVarToDeclare() {
    assert(hasInitializedVarsToDeclare() && "Error!");

    auto p = _varsInitializedToBeDeclared.front();
    _varsInitializedToBeDeclared.pop();

    return p;
}

pair<string, uint16_t> SSANumberingTable::popUnInitializedVarToDeclare() {
    assert(hasUnInitializedVarsToDeclare() && "Error!");

    auto p = _varsUnIntializedToBeDeclared.front();
    _varsUnIntializedToBeDeclared.pop();

    return p;
}
