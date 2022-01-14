#pragma once
#include "VariableTable.h"
#include "SMTSolverBase.h"

namespace weaver {

    class Yices : public SMTSolverBase {
    public:
        Yices(VariableTable* table) :
            SMTSolverBase(table)
        {}

    private:
        string getCommand(const string& SMTFile) const override
        { return "echo '" + SMTFile + "'|" + YICES_BIN_PATH; }

    };
}



