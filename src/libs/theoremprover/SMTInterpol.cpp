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

Interpolants SMTInterpol::extractInterpolants(const string& line) const {
    Interpolants interpolants;
    interpolants.push_back("true");

    // we need extract the interpolants from the interpolants string
    int searchStartIndex = 0;
    int searchEndIndex = 0;
    // the stack that stores the open and end parens
    vector<char> parenStack;
    while (searchEndIndex < line.size()) {
        while (line.at(searchStartIndex) == ' ')
            searchStartIndex ++;

        char currentChar = line.at(searchEndIndex);

        if (currentChar == '(') {
            parenStack.push_back('(');
        }
        else if (currentChar == ')') {
            if (parenStack.empty() || parenStack.at(parenStack.size()-1) != '(') {
                cerr << "Stack size: " << parenStack.size() << endl;
                cerr << "SearchEndIndex: " << searchEndIndex << endl;
                assert(false && "Error in popping parenStack!\n");
            }
            parenStack.pop_back();

            if (parenStack.empty()) {
                string intpl = line.substr(searchStartIndex, searchEndIndex-searchStartIndex+1);
                removeSSANumberingPortion(intpl);
                interpolants.push_back(intpl);
                searchStartIndex = searchEndIndex + 1;
            }

        }
        else if (currentChar == 't' && parenStack.empty()) {
            interpolants.push_back(line.substr(searchStartIndex, 4));
            searchEndIndex += 3;
            searchStartIndex = searchEndIndex + 1;
        }
        else if (currentChar == 'f' && parenStack.empty()) {
            interpolants.push_back(line.substr(searchStartIndex, 5));
            searchEndIndex += 4;
            searchStartIndex = searchEndIndex + 1;
        }

        searchEndIndex ++;
    }

    interpolants.push_back("false");

    return interpolants;
}


