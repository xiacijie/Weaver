#include "MathSAT.h"

using namespace weaver;

string MathSAT::setInterpolationOptions() const {
    stringstream ss;
    ss << setOption("produce-interpolants", getTrue());
    ss << endl;
    return ss.str();
}


Interpolants MathSAT::processInterpolationResult(const string &result) const {
    if (result.substr(0, 6) == "(error") {
        assert(false && "Error with the theorem prover!\n");
    }

    if (result.at(0) != 'u') { // if the SMT solver returns sat, then there is a counterexample
        // return the empty interpolants
        return {};
    }

    vector<string> lines = util::tokenize(result, '\n');

    Interpolants interpolants;
    interpolants.push_back("true");
    for (int i = 1; i < lines.size(); i++) {
        removeSSANumberingPortion(lines[i]);
        interpolants.push_back(lines[i]);
    }
    interpolants.push_back("false");

    return interpolants;
}
