#include "SMTInterpol.h"
#include <iostream>
#include <sstream>
#include <vector>

using namespace weaver;
using namespace std;

bool SMTInterpol::checkIndependenceRelation(Statement *s1, Statement *s2) const {
    if (s1->getThreadID() == 0 || s2->getThreadID() == 0) {
        return false;
    }

    if (s1->getThreadID() == s2->getThreadID()) { // statements in the same thread are dependent anyway
        return false;
    }

    if (s1->getNode()->isBool() || s2->getNode()->isBool()) { // we do not handle assertions now
        return false;
    }

    stringstream SMTFile;
    SMTFile << setOption("print-success", getFalse());
    SMTFile << setLogic("QF_UFLIA") << endl;

    SSANumberingTable ssaNumberingTable;

    // generate the SMT formulas for s1s2
    string s1Formula1 = getFormula(s1->getNode(), s1, ssaNumberingTable);
    string s2Formula1 = getFormula(s2->getNode(), s2, ssaNumberingTable);

    vector<pair<string, uint16_t>> varsNotInitialized1;
    while (!ssaNumberingTable.isNotInitializedVarQueueEmpty()) {
        auto varNotInit = ssaNumberingTable.popNotInitializedVar();
        SMTFile << getVarDeclaration(getSSAVarName(varNotInit.first,varNotInit.second), _vTable->getVarType(varNotInit.first));
        SMTFile << endl;
        varsNotInitialized1.push_back(varNotInit);
    }

    unordered_map<string, uint16_t> largestVarNumber1;
    while (!ssaNumberingTable.isInitializedVarQueueEmpty()) {
        pair<string, uint16_t> varInit = ssaNumberingTable.popInitializedVar();
        SMTFile << getVarDeclaration(getSSAVarName(varInit.first, varInit.second), _vTable->getVarType(varInit.first));
        SMTFile << endl;

        if (largestVarNumber1.find(varInit.first) == largestVarNumber1.end()) {
            largestVarNumber1[varInit.first] = varInit.second;
        }
        else {
            largestVarNumber1[varInit.first] = max(largestVarNumber1[varInit.first], varInit.second);
        }
    }

    // let all vars uninitialized but keep the ssa numberings
    ssaNumberingTable.uninitializeAllVars();

    // generate the SMT formulas for s2s1
    string s2Formula2 = getFormula(s2->getNode(), s2, ssaNumberingTable);
    string s1Formula2 = getFormula(s1->getNode(), s1, ssaNumberingTable);

    vector<pair<string, uint16_t>> varsNotInitialized2;
    while (!ssaNumberingTable.isNotInitializedVarQueueEmpty()) {
        auto varNotInit = ssaNumberingTable.popNotInitializedVar();
        SMTFile << getVarDeclaration(getSSAVarName(varNotInit.first,varNotInit.second), _vTable->getVarType(varNotInit.first));
        SMTFile << endl;
        varsNotInitialized2.push_back(varNotInit);
    }

    unordered_map<string, uint16_t> largestVarNumber2;
    while (!ssaNumberingTable.isInitializedVarQueueEmpty()) {
        pair<string, uint16_t> varInit = ssaNumberingTable.popInitializedVar();
        SMTFile << getVarDeclaration(getSSAVarName(varInit.first, varInit.second), _vTable->getVarType(varInit.first));
        SMTFile << endl;
        if (largestVarNumber2.find(varInit.first) == largestVarNumber2.end()) {
            largestVarNumber2[varInit.first] = varInit.second;
        }
        else {
            largestVarNumber2[varInit.first] = max(largestVarNumber2[varInit.first], varInit.second);
        }
    }

    SMTFile << getAssert(getAndFormula(s1Formula1, s2Formula1)) << endl;
    SMTFile << getAssert(getAndFormula(s2Formula2, s1Formula2)) << endl;

    for (const auto & v1 : varsNotInitialized1) {
        for (const auto& v2 : varsNotInitialized2) {
            if (v1.first == v2.first) {
                SMTFile << getAssert(getEqFormula(getSSAVarName(v1.first, v1.second), getSSAVarName(v2.first, v2.second)));
                SMTFile << endl;
            }
        }
    }


    string conjunctEQ;
    for (const auto& it : largestVarNumber1) {
        string varName = it.first;

        if (conjunctEQ.empty()) {
            conjunctEQ = getEqFormula(
                            getSSAVarName(varName, largestVarNumber1[varName]),
                            getSSAVarName(varName, largestVarNumber2[varName]));
        }
        else {
            conjunctEQ = getAndFormula(conjunctEQ,
                                       getEqFormula(
                                            getSSAVarName(varName, largestVarNumber1[varName]),
                                            getSSAVarName(varName, largestVarNumber2[varName])
                    ));
        }
    }

    SMTFile << getAssert(getNotFormula(conjunctEQ))  << endl;

    SMTFile << checkSat() << endl;

    string result = exec(getCommand(SMTFile.str()));

    if (result.at(0) =='u')
        return true;

    return false;
}

Interpolants SMTInterpol::generateInterpols(const Trace& trace) const {

    SSANumberingTable ssaNumberingTable;

    stringstream SMTFile;

    SMTFile << setOption("print-success", getFalse());
    SMTFile << setOption("produce-interpolants", getTrue());
    SMTFile << setLogic("QF_UFLIA");
    SMTFile << endl;

    for (int i = 0; i < trace.size(); i ++) {
        Statement* stmt = trace[i];
        ASTNode* node = stmt->getNode();

        string formula = getFormula(node, stmt, ssaNumberingTable);

        if (!ssaNumberingTable.isNotInitializedVarQueueEmpty()) {
            while (!ssaNumberingTable.isNotInitializedVarQueueEmpty()) {
                auto var = ssaNumberingTable.popNotInitializedVar();
                string varDecl = getVarDeclaration(getSSAVarName(var.first, var.second), _vTable->getVarType(var.first));
                SMTFile << varDecl;
                SMTFile << endl;
            }
        }

        if (!ssaNumberingTable.isInitializedVarQueueEmpty()) { // need to declare those vars
            while (!ssaNumberingTable.isInitializedVarQueueEmpty()) {
                auto newSSAVar = ssaNumberingTable.popInitializedVar();
                string ssaVarName = getSSAVarName(newSSAVar.first, newSSAVar.second);
                string varDecl = getVarDeclaration(ssaVarName, _vTable->getVarType(newSSAVar.first));
                SMTFile << varDecl;
                SMTFile << endl;
            }
        }

        SMTFile << getAssert(labelFormula(formula, FORMULA_LABEL + to_string(i)));
        SMTFile << endl;
    }

    SMTFile << checkSat();
    SMTFile << endl;

    vector<string> labels;

    labels.reserve(trace.size());
    for (int i = 0; i < trace.size(); i ++) {
        labels.push_back(FORMULA_LABEL + to_string(i));
    }

    SMTFile << getInterpolants(labels);

    string result = exec(getCommand(SMTFile.str()));

    if (result.substr(0, 6) == "(error") {
        assert(false && "Error with the theorem prover!\n");
    }

    if (result.at(0) != 'u') { // if the SMT solver returns sat, then there is a counterexample
        // return the empty interpolants
        Interpolants  interpolants;
        return interpolants;
    }

    int i = 0;
    while (result.at(i) != '(') {
        i ++;
    }

    string interpolantsString = result.substr(i+1, result.size()-i-1-2);

    return extractInterpolants(interpolantsString);
}

bool SMTInterpol::entails(const string &formula1, const string &formula2) const{
    stringstream SMTFile;

    SMTFile << setOption("print-success", getFalse());
    SMTFile << setLogic("QF_UFLIA");
    SMTFile << endl;

    for (auto v : _vTable->getGlobalVariables()) {
        SMTFile << getVarDeclaration(v.first, v.second) << endl;
    }

    SMTFile << getAssert(getNotFormula(getImplyFormula(formula1, formula2)));
    SMTFile << endl;
    SMTFile << checkSat();

    string result = exec(getCommand(SMTFile.str()));

    if (result.substr(0, 6) == "(error") {
        assert(false && "Error with the theorem prover!\n");
    }

    if (result.at(0) == 'u')
        return true;

    return false;
}


