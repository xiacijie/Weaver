#pragma once
#include "TheoremProverBase.h"

namespace weaver {
    class Program;

    class Yices : public TheoremProverBase {
    public:
        Yices(Program* program) :
            TheoremProverBase(program)
        {}

    private:
        string getCommand(const string& SMTFile) const override
        { return "echo '" + SMTFile + "'|" + YICES_BIN_PATH; }

    };
}



