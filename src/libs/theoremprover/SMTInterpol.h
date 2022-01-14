#pragma once
#include "SMTSolverBase.h"
#include "VariableTable.h"

namespace weaver {

    class SMTInterpol : public SMTSolverBase{
    public:
        SMTInterpol(VariableTable* table) :
                SMTSolverBase(table)
            {}

    private:
        string setInterpolationOptions() const override;
        string setEntailmentOptions() const override;
        Interpolants processInterpolationResult(const string& result) const override;

        string getCommand(const string &SMTFile) const override
        { return "echo '" + SMTFile + "'|" + "java -jar " + SMT_INTERPOL_BIN_PATH + " -q "; }
    };
}




