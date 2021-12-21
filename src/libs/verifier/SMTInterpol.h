#pragma once
#include "TheoremProverBase.h"
#include "../program/Program.h"


namespace weaver {
    class SMTInterpol : public TheoremProverBase{
    public:
        SMTInterpol(Program* program) :
                TheoremProverBase(),
                _vTable(&program->getVariableTable())
            {}

        Interpolants generateInterpols(const Trace& trace) const override;
        bool entails(const string &formula1, const string &formula2) const override;
        bool checkIndependenceRelation(Statement* s1, Statement* s2) const override;
        bool checkHoareTripe(const string& pre, Statement* statement, const string& post) const  override;

    private:
        string getCommand(const string &SMTFile) const override
        { return "echo '" + SMTFile + "'|" + "java -jar " + SMT_INTERPOL_BIN_PATH + " -q "; }

        VariableTable* _vTable;
    };
}




