#include "SMTInterpol.h"
#include <iostream>

using namespace weaver;
using namespace std;

string SMTInterpol::setInterpolationOptions() const {
    stringstream ss;
    ss << setOption("print-success", getFalse());
    ss << setOption("produce-interpolants", getTrue());
    ss << setLogic("QF_AUFLIA");
    ss << setOption("simplify-interpolants", getTrue());
    ss << setOption("print-terms-cse", getFalse());
    ss << endl;
    return ss.str();
}

Interpolants SMTInterpol::processInterpolationResult(const string &result) const {
    if (result.substr(0, 6) == "(error") {
        assert(false && "Error with the theorem prover!\n");
    }

    if (result.at(0) != 'u') { // if the SMT solver returns sat, then there is a counterexample
        // return the empty interpolants
        return {};
    }

    int i = 0;
    while (result.at(i) != '(') {
        i ++;
    }

    string interpolantsString = result.substr(i+1, result.size()-i-1-2);

    return extractInterpolants(interpolantsString);
}

string SMTInterpol::setEntailmentOptions() const {
    stringstream ss;
    ss << setOption("print-success", getFalse());
    ss << setLogic("QF_AUFLIA");
    ss << endl;
    return ss.str();
}


