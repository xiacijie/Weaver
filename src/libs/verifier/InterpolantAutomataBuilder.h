#pragma once
#include "TheoremProverBase.h"
#include "../automata/NFA.h"

namespace weaver {
    class InterpolantAutomataBuilder {
    public:
        static NFA* build(const Trace &errorTrace, const Interpolants &interpols, TheoremProverBase* prover, Program* program);
    };
}