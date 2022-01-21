#pragma once
#include "ASTNode.h"
#include <unordered_map>

namespace weaver {
    /**
    * The Table for storing variable information.
    */
    class VariableTable {

    public:
        void declareVar(const string &varName, DataType type);
        void initializeVar(const string &varName);

        DataType getVarType(const string &varName);

        bool isVarDeclared(const string &varName);
        bool isVarInitialized(const string &varName);

        const unordered_map<string, DataType>& getTable() { return _varTable; }

        string toString();

    private:
        // the uint16_t represents the scope number: 0 is the global scope
        unordered_map<string, DataType> _varTable;
        unordered_map<string, bool> _varInitializationTable;
    };

}