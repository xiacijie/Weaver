#include "MathSAT.h"

using namespace weaver;

string MathSAT::setInterpolationOptions() const {
    stringstream ss;
    ss << setOption("produce-interpolants", getTrue());;
    ss << endl;
    return ss.str();
}

string MathSAT::setEntailmentOptions() const {
    stringstream ss;
    ss << setLogic("QF_AUFLIA");
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

    for (auto& line: lines) {
        removeSSANumberingPortion(line);
    }

    Interpolants interpolants;
    interpolants.push_back("true");
    for (int i = 1; i < lines.size(); i++) {
        interpolants.push_back(lines[i]);
    }
    interpolants.push_back("false");

    return interpolants;
}
