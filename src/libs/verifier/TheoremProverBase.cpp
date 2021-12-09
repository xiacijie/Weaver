#include "TheoremProverBase.h"
#include <cstdio>
#include <memory>

#define BUFFER_SIZE 128

using namespace std;
using namespace weaver;

void SSANumberingTable::declareNotInitializedVar(const string &varName) {
    if (!inNumberTable(varName)) {
        _numberTable[varName] = 0;
    }
    else {
        _numberTable[varName]++;
    }

    enqueueVarNotInitialized(make_pair(varName, _numberTable[varName]));
}

uint16_t SSANumberingTable::getVersionNumber(const string &varName) {
    if (!inNumberTable(varName))
        return 0;

    return _numberTable[varName];
}

void SSANumberingTable::declareInitializedVar(const string &varName,Statement* stmt) {
    if (!inNumberTable(varName)) {
        _numberTable[varName] = 0;
    }
    else {
        _numberTable[varName]++;
    }

    enqueueVarInitialized(make_pair(varName, _numberTable[varName]));
    initializeVar(varName, stmt);
}

Statement* SSANumberingTable::getInitializationStatement(const string &varName) {
    const auto& it = _varsInitializedMap.find(varName);
    assert(it != _varsInitializedMap.end() && "Error");

    return it->second;
}

pair<string, uint16_t> SSANumberingTable::popInitializedVar() {
    assert(!isInitializedVarQueueEmpty() && "Error");

    auto newVar = _varsInitialized.front();
    _varsInitialized.pop();

    return newVar;
}

pair<string, uint16_t> SSANumberingTable::popNotInitializedVar() {
    assert(!isNotInitializedVarQueueEmpty() && "Empty");
    auto var = _varsNotInitialized.front();
    _varsNotInitialized.pop();

    return var;
}

void SSANumberingTable::uninitializeVar(const string &varName) {
    assert(!isVarInitialized(varName) && "Error");

    _varsInitializedMap.erase(_varsInitializedMap.find(varName));
}

void SSANumberingTable::uninitializeAllVars() {
    _varsInitializedMap.clear();
}

string TheoremProverBase::exec(const string &command) const {
    shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe)
        assert(false && "Pipe failed to open!\n");

    char buffer[BUFFER_SIZE];

    string result = "";
    while (!feof(pipe.get())) {
        if (fgets(buffer, BUFFER_SIZE, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}

string TheoremProverBase::getVarDeclaration(const string &varName, DataType dType) const {
    stringstream ss;
    ss << "declare-const" << " " << varName << " ";
    switch (dType) {
        case DataType::Int: ss << "Int"; break;
        case DataType::Bool: ss << "Bool"; break;
        default:
            assert(false && "Error in getVarDeclaration!\n");
            break;
    }
    string decl = parenEnclose(ss.str());
    return decl;
}

string TheoremProverBase::getFormula(ASTNode *node, Statement* stmt, SSANumberingTable& ssaNumberingTable, bool isLeftValue, bool insideAssume) const {
    // Left value is the left-hand side of an assignment statement
    // Need to update the ssa numbering table
    if (isLeftValue) {
        string varName = node->getIdName();
        ssaNumberingTable.declareInitializedVar(varName, stmt);
    }

    if (node->isNoAction()) {
        return getTrue();
    }

    if (node->isAtomic()) {
        if (node->getNumChildren() == 1) {
            return getFormula(node->getChild(0), stmt, ssaNumberingTable);
        }

        vector<string> statementFormulas;
        statementFormulas.reserve(node->getNumChildren());
        for (int i = 0; i < node->getNumChildren(); i ++) {
            ASTNode* child = node->getChild(i);
            statementFormulas.push_back(getFormula(child, stmt, ssaNumberingTable));
        }

        string conjunctStatements = getAndFormula(statementFormulas[0], statementFormulas[1]);
        for (int i = 2; i < statementFormulas.size(); i ++) {
            conjunctStatements = getAndFormula(conjunctStatements, statementFormulas[i]);
        }

        return conjunctStatements;
    }

    if (node->isAssume()) {
        return getFormula(node->getChild(0), stmt, ssaNumberingTable, false, true);
    }

    if (node->isId()) { // get the ssa version of this variable
        string varName = node->getIdName();

        if (!ssaNumberingTable.isVarInitialized(varName)) { // first time encounter this var
            ssaNumberingTable.declareNotInitializedVar(varName); //should be set to 0
        }

        string ssaVarName = getSSAVarName(varName, ssaNumberingTable.getVersionNumber(varName));
        return ssaVarName;
    }

    if (node->isIntConst())
        return to_string(node->getInt());

    if (node->isBoolConst()) {
        switch (node->getBool()) {
            case BooleanValue::True: return getTrue();
            case BooleanValue::False: return getFalse();
            case BooleanValue::NonDeter:
                assert(false&& "Non deter bool value should not occur here!\n");
                return nullptr;
        }
    }

    if (node->getNodeType() == NodeType::Not) {
        return getNotFormula(getFormula(node->getChild(0), stmt, ssaNumberingTable));
    }
    else if (node->getNodeType() == NodeType::Negation) {
        return getNegationFormula(getFormula(node->getChild(0), stmt, ssaNumberingTable));
    }
    else {
        string left;
        string right;

        if (node->isAssign()) {
            right = getFormula(node->getChild(1), stmt, ssaNumberingTable, false, insideAssume);
            left = getFormula(node->getChild(0), stmt, ssaNumberingTable, true, insideAssume);
        }
        else if (insideAssume) {
            if (node->getChild(0)->isId()) {
                const string& varName = node->getChild(0)->getIdName();
                if (!ssaNumberingTable.isVarInitialized(varName)) {
                    right = getFormula(node->getChild(1), stmt, ssaNumberingTable, false, insideAssume);
                    left = getFormula(node->getChild(0), stmt, ssaNumberingTable, true, insideAssume);
                }
                else {
                    Statement* initializationStatement = ssaNumberingTable.getInitializationStatement(varName);
                    if (initializationStatement != stmt) { // different statements, introduce new ssa numbers
                        right = getFormula(node->getChild(1), stmt, ssaNumberingTable, false, insideAssume);
                        left = getFormula(node->getChild(0), stmt, ssaNumberingTable, true, insideAssume);
                    }
                }
            }
        }

        if (left.empty() && right.empty()) {
            left = getFormula(node->getChild(0), stmt, ssaNumberingTable, false, insideAssume);
            right = getFormula(node->getChild(1), stmt, ssaNumberingTable, false, insideAssume);
        }



        switch (node->getNodeType()) {
            case NodeType::Addition:
                return getAddFormula(left, right);
            case NodeType::Subtraction:
                return getMinusFormula(left, right);
            case NodeType::Multiplication:
                return getMultFormula(left, right);
            case NodeType::Assignment:
                return getEqFormula(left, right);
            case NodeType::CompareLT:
                return getLTFormula(left, right);
            case NodeType::CompareLTE:
                return getLTEFormula(left, right);
            case NodeType::CompareGT:
                return getGTFormula(left, right);
            case NodeType::CompareGTE:
                return getGTEFormula(left, right);
            case NodeType::CompareEQ:
                return getEqFormula(left, right);
            case NodeType::CompareNEQ:
                return getNeqFormula(left, right);
            case NodeType::And:
                return getAndFormula(left, right);
            case NodeType::Or:
                return getOrFormula(left, right);
            default:
                cerr << NodeTypeLabels[node->getNodeType()] << endl;
                assert(false && "Error in getFormula!\n");
        }
    }
}

Interpolants TheoremProverBase::extractInterpolants(const string &line) const {
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

void TheoremProverBase::removeSSANumberingPortion(string& interpolant) const{
    bool ssaPortionFound = false;
    int startIndex = 0;
    int endIndex = 0;

    while (true) {
        int length = interpolant.size();

        if (endIndex >= length) {
            break;
        }

        char currentChar = interpolant.at(endIndex);

        if (currentChar == SSA_DELIMITER) {
            // make sure the '-' is not the subtraction
            if (!(interpolant.at(endIndex-1) == ' ' || interpolant.at(endIndex-1) == '(')) {
                ssaPortionFound = true;
                startIndex = endIndex;
            }

        }
        else if (ssaPortionFound && (!isdigit(currentChar) || endIndex == length - 1)) {
            int cutLength = endIndex - startIndex;
            interpolant.erase(startIndex, cutLength);

            endIndex = startIndex - 1;
            ssaPortionFound = false;
        }

        endIndex++;
    }
}