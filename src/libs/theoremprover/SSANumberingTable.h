#pragma once

#include <unordered_map>
#include <queue>
#include "ASTNode.h"

#define SSA_DELIMITER '-'
#define FORMULA_LABEL "L"

using namespace std;

namespace weaver {
    class SSANumberingTable {
    public:
        SSANumberingTable() {}
        uint16_t getNumber(const string& varName);
        bool hasVar(const string& varName);

        // declare vars that are initialized by assignment
        void declareDefinedVar(const string& varName);

        // declare vars that are not initialized
        void declareUndefinedVar(const string& varName);

        pair<string, uint16_t> popVarToDeclare();
        pair<string, uint16_t> popDefinedVarToDeclare();
        pair<string, uint16_t> popUndefinedVarToDeclare();

        bool hasVarsToDeclare() { return hasDefinedVarsToDeclare() || hasUndefinedVarsToDeclare();}
        bool hasDefinedVarsToDeclare() { return !_varsDefinedToBeDeclared.empty(); }
        bool hasUndefinedVarsToDeclare() { return !_varsUndefinedToBeDeclared.empty(); }

        void setNumber(const string& varName, uint16_t i) { _numberTable[varName] = i; } 
    private:
        void incNumber(const string& varName);
        
        unordered_map<string, uint16_t> _numberTable;
        queue<pair<string, uint16_t>> _varsDefinedToBeDeclared;
        queue<pair<string, uint16_t>> _varsUndefinedToBeDeclared;

    };
}