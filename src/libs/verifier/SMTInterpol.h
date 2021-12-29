#pragma once
#include "TheoremProverBase.h"
#include "../program/Program.h"
#include "../ast/VariableTable.h"

namespace weaver {
    class SMTInterpol : public TheoremProverBase{
    public:
        SMTInterpol(Program* program) :
                TheoremProverBase(program)
            {}

        bool checkIndependenceRelation(Statement* s1, Statement* s2) const override;
        bool checkHoareTripe(const string& pre, Statement* statement, const string& post) const override;

    private:
        string setInterpolationOptions() const override;
        string setEntailmentOptions() const override;
        Interpolants processInterpolationResult(const string& result) const override;

        string getCommand(const string &SMTFile) const override
        { return "echo '" + SMTFile + "'|" + "java -jar " + SMT_INTERPOL_BIN_PATH + " -q "; }
    };
}




