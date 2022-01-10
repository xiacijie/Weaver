#pragma once
#include "TheoremProverBase.h"
#include "VariableTable.h"

namespace weaver {
    class Program;

    class SMTInterpol : public TheoremProverBase{
    public:
        SMTInterpol(Program* program) :
                TheoremProverBase(program)
            {}

    private:
        string setInterpolationOptions() const override;
        string setEntailmentOptions() const override;
        Interpolants processInterpolationResult(const string& result) const override;

        string getCommand(const string &SMTFile) const override
        { return "echo '" + SMTFile + "'|" + "java -jar " + SMT_INTERPOL_BIN_PATH + " -q "; }
    };
}




