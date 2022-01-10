#pragma once

#include "TheoremProverBase.h"
#include "VariableTable.h"
#include "String.h"

using namespace util;

namespace weaver {
    class program;

    class MathSAT : public TheoremProverBase {
    public:
        MathSAT(Program* program) :
            TheoremProverBase(program)
            {}
    private:
        string setInterpolationOptions() const override;

        Interpolants processInterpolationResult(const string& result) const override;

        string getCommand(const string &SMTFile) const override
        { return "echo '" + SMTFile + "'|" + MATH_SAT_BIN_PATH; }

        string labelFormula(const string& formula, const string& label) const override
        { return parenEnclose(string("!") + " " + formula + " " + ":interpolation-group" + " " + label);}

        string getInterpolants(const vector<string>& labels) const override
        {
            stringstream ss;
            for (int i = 1; i < labels.size(); i++) {
                vector<string> sub(labels.begin(), labels.begin() + i);
                ss << parenEnclose("get-interpolant " + parenEnclose(util::join(sub, " ")));
            }
            return ss.str();
        }
    };

}

