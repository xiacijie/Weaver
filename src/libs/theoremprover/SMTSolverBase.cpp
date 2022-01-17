#include "SMTSolverBase.h"
#include <iostream>

using namespace weaver;
using namespace std;

string SMTSolverBase::getFormula(ASTNode* node, SSANumberingTable& table, bool isLeftValue) const  {
    // Left value is the left-hand side of an assignment statement
    // Need to update the ssa numbering table
    if (isLeftValue) {
        string varName = node->getIdName();
        table.declareDefinedVar(varName);
    }

    if (node->isNoAction()) {
        return getTrue();
    }

    if (node->isAtomic()) {
        if (node->getNumChildren() == 1) {
            return getFormula(node->getChild(0), table);
        }

        vector<string> statementFormulas;
        statementFormulas.reserve(node->getNumChildren());
        for (int i = 0; i < node->getNumChildren(); i ++) {
            ASTNode* child = node->getChild(i);
            statementFormulas.push_back(getFormula(child, table));
        }

        string conjunctStatements = getAndFormula(statementFormulas[0], statementFormulas[1]);
        for (int i = 2; i < statementFormulas.size(); i ++) {
            conjunctStatements = getAndFormula(conjunctStatements, statementFormulas[i]);
        }

        return conjunctStatements;
    }

    if (node->isAssume()) {
        return getFormula(node->getChild(0), table);
    }

    if (node->isId()) { // get the ssa version of this var
        string varName = node->getIdName();
        if (!table.hasVar(varName)) {
            table.declareUndefinedVar(varName);
        }

        return getSSAName(varName, table.getNumber(varName));
    }

    if (node->isIntConst()) {
        return to_string(node->getInt());
    }

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
        return getNotFormula(getFormula(node->getChild(0), table));
    }

    if (node->getNodeType() == NodeType::Negation) {
        return getNegationFormula(getFormula(node->getChild(0), table));
    }

    //evaluate the right-hand side first, then the left-hand side
    string right = getFormula(node->getChild(1), table);
    if (node->isAssign()) {
        isLeftValue = true;
    }

    string left = getFormula(node->getChild(0), table, isLeftValue);

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

string SMTSolverBase::setEntailmentOptions() const {
    stringstream ss;
    ss << setOption("print-success", getFalse());
    ss << setLogic("QF_AUFLIA");
    ss << endl;
    return ss.str();
}

string SMTSolverBase::setCheckHoareTripeOptions() const {
    stringstream ss;
    ss << setOption("print-success", getFalse());
    ss << setLogic("QF_AUFLIA");
    ss << endl;
    return ss.str();
}

string SMTSolverBase::setCheckIndependenceOptions() const {
    stringstream ss;
    ss << setOption("print-success", getFalse());
    ss << setLogic("QF_UFLIA");
    ss << endl;
    return ss.str();
}

string SMTSolverBase::getVarDeclaration(const string &varName, DataType dType) const {
    stringstream ss;
    ss << "declare-const" << " " << varName << " ";
    switch (dType) {
        case DataType::Int: ss << "Int"; break;
        case DataType::Bool: ss << "Bool"; break;
        case DataType::IntArray: ss << "(Array Int Int)"; break;
        default:
            assert(false && "Error in getVarDeclaration!\n");
            break;
    }
    string decl = parenEnclose(ss.str());
    return decl;
}

Interpolants SMTSolverBase::generateInterpols(const Trace &trace) const {
    SSANumberingTable table;
    stringstream SMTFile;

    SMTFile << setInterpolationOptions();

    for (int i = 0; i < trace.size(); i ++) {
        Statement* stmt = trace[i];
        ASTNode* node = stmt->getNode();

        string formula = getFormula(node, table);

        while (table.hasVarsToDeclare()) {
            auto v = table.popVarToDeclare();
            SMTFile << getVarDeclaration(getSSAName(v.first, v.second), _vTable->getVarType(v.first));
            SMTFile << endl;
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

    SMTFile << getInterpolants(labels) << endl;
    string result = exec(getCommand(SMTFile.str()));

    return processInterpolationResult(result);
}

bool SMTSolverBase::checkIndependenceRelation(Statement* s1, Statement* s2) const  {
    if (s1->getThreadID() == 0 || s2->getThreadID() == 0) {
        return false;
    }

    if (s1->getThreadID() == s2->getThreadID()) {
        return false;
    }

    stringstream SMTFile;
    SMTFile << setCheckIndependenceOptions();

    // generate the SMT formulas for s1s2
    SSANumberingTable table1;

    string s1Formula1 = getFormula(s1->getNode(), table1);
    string s2Formula1 = getFormula(s2->getNode(), table1);  

    // vars that are not init by assignment statements should be assert equal
    vector<pair<string, uint16_t>> varsNotInitialized1;
    while (table1.hasUndefinedVarsToDeclare()) {
        auto v = table1.popUndefinedVarToDeclare();
        SMTFile << getVarDeclaration(getSSAName(v.first,v.second), _vTable->getVarType(v.first));
        SMTFile << endl;
        varsNotInitialized1.push_back(v);
    }

    // initialized vars' largest version number
    unordered_map<string, uint16_t> varsInitialized1;
    while (table1.hasDefinedVarsToDeclare()) {
        auto v = table1.popDefinedVarToDeclare();
        SMTFile << getVarDeclaration(getSSAName(v.first, v.second), _vTable->getVarType(v.first));
        SMTFile << endl;

        if (varsInitialized1.find(v.first) == varsInitialized1.end()) {
            varsInitialized1[v.first] = v.second;
        }
        else {
            varsInitialized1[v.first] = max(varsInitialized1[v.first], v.second);
        }
    }

    // generate the SMT formulas for s2s1
    SSANumberingTable table2;

    SSANumberingTable temp;
    getFormula(s2->getNode(), temp);
    getFormula(s1->getNode(), temp);

    vector<pair<string, uint16_t>> varsNotInitialized2;
    while (temp.hasUndefinedVarsToDeclare()) {
        auto v = temp.popUndefinedVarToDeclare();
        uint16_t versionNumber = table1.getNumber(v.first) + 1;
        table2.setNumber(v.first, versionNumber);
        SMTFile << getVarDeclaration(getSSAName(v.first, versionNumber), _vTable->getVarType(v.first));
        SMTFile << endl;
        varsNotInitialized2.push_back(make_pair(v.first, versionNumber));
    }

    unordered_map<string, uint16_t> varsInitialized2;
    while (temp.hasDefinedVarsToDeclare()) {
        auto v = temp.popDefinedVarToDeclare();
        uint16_t versionNumber = table1.getNumber(v.first) + v.second + 1;
        
        if (!table2.hasVar(v.first)) {
            table2.setNumber(v.first, table1.getNumber(v.first));
        }
    
        SMTFile << getVarDeclaration(getSSAName(v.first, versionNumber), _vTable->getVarType(v.first));
        SMTFile << endl;

        if (varsInitialized2.find(v.first) == varsInitialized2.end()) {
            varsInitialized2[v.first] = versionNumber;
        }
        else {
            varsInitialized2[v.first] = max(varsInitialized2[v.first], versionNumber);
        }
    }

    string s2Formula2 = getFormula(s2->getNode(), table2);
    string s1Formula2 = getFormula(s1->getNode(), table2);

    assert(!table2.hasUndefinedVarsToDeclare() && "Should not have uninitialized vars");

    // construct the SMT formulas
    SMTFile << getAssert(getAndFormula(s1Formula1, s2Formula1)) << endl;
    SMTFile << getAssert(getAndFormula(s2Formula2, s1Formula2)) << endl;

        for (const auto & v1 : varsNotInitialized1) {
        for (const auto& v2 : varsNotInitialized2) {
            if (v1.first == v2.first) {
                SMTFile << getAssert(getEqFormula(getSSAName(v1.first, v1.second), getSSAName(v2.first, v2.second)));
                SMTFile << endl;
            }
        }
    }

    string conjunctEQ;
    for (const auto& it : varsInitialized1) {
        string varName = it.first;

        if (conjunctEQ.empty()) {
            conjunctEQ = getEqFormula(
                    getSSAName(varName, varsInitialized1[varName]),
                    getSSAName(varName, varsInitialized2[varName]));
        }
        else {
            conjunctEQ = getAndFormula(conjunctEQ,
                                       getEqFormula(
                                               getSSAName(varName, varsInitialized1[varName]),
                                               getSSAName(varName, varsInitialized2[varName])
                                       ));
        }
    }
    
    if (conjunctEQ.empty()) {
        conjunctEQ = getTrue();
    }

    SMTFile << getAssert(getNotFormula(conjunctEQ))  << endl;
    
    
    SMTFile << checkSat() << endl;

    string result = exec(getCommand(SMTFile.str()));

    if (result.substr(0, 6) == "(error") {
        assert(false && "Error with the theorem prover!\n");
    }

    if (result.at(0) =='u')
        return true;

    return false;
}

bool SMTSolverBase::checkHoareTripe(const string &pre, Statement *statement, const string &post) const {
    // pre-condition is false, then always valid
    if (pre == getFalse()) {
        return true;
    }

    // post-condition is true, then always valid
    if (post == getTrue()) {
        return true;
    }

    SSANumberingTable table;

    stringstream SMTFile;
    SMTFile << setCheckHoareTripeOptions();
    SMTFile << endl;

    string preCondition = pre;
    string postCondition = post;

    // update the ssa numbering of the pre-condition assertion
    int startIndex = 0;
    int endIndex = 0;

    while (true) {
        int length = preCondition.size();
        if (endIndex >= length) {
            break;
        }

        char currentChar = preCondition.at(endIndex);
        if (currentChar == ' ' || currentChar == '(' || currentChar == ')' || endIndex == length - 1) {
            if (endIndex >= startIndex) {
                int cutLength = endIndex - startIndex;

                if (endIndex == length - 1 && currentChar != ')') {
                    cutLength ++;
                }

                string part = preCondition.substr(startIndex, cutLength);

                // if the part is a variable
                if (_vTable->isVarDeclared(part)) {
                    if (!table.hasVar(part)) {
                        table.declareUndefinedVar(part);
                    }

                    if (endIndex == length - 1 && currentChar != ')') {
                        preCondition.insert(endIndex+1, SSA_DELIMITER + to_string(0));
                    }
                    else {
                        preCondition.insert(endIndex, SSA_DELIMITER + to_string(0));
                    }

                    endIndex += 2;
                }
            }

            startIndex = endIndex + 1;
        }

        endIndex++;
    }

    while (table.hasVarsToDeclare()) {
        auto v = table.popVarToDeclare();
        SMTFile << getVarDeclaration(getSSAName(v.first, v.second), _vTable->getVarType(v.first));
        SMTFile << endl;
    }

    SMTFile << getAssert(preCondition) << endl;
    string formula;

    // get the SMT formula for the statement
    if (statement == nullptr) { // although should never be null
        formula = getTrue();
    }
    else {
        formula = getFormula(statement->getNode(), table);
    }

    while (table.hasVarsToDeclare()) {
        auto v = table.popVarToDeclare();
        SMTFile << getVarDeclaration(getSSAName(v.first, v.second), _vTable->getVarType(v.first));
        SMTFile << endl;
    }

    SMTFile << getAssert(formula) << endl;

    startIndex = 0;
    endIndex = 0;
    while (true) {
        int length = postCondition.size();
        if (endIndex >= length) {
            break;
        }

        char currentChar = postCondition.at(endIndex);
        if (currentChar == ' ' || currentChar == '(' || currentChar == ')' || endIndex == length - 1) {
            if (endIndex >= startIndex) {
                int cutLength = endIndex - startIndex;

                if (endIndex == length - 1 && currentChar != ')') {
                    cutLength++;
                }

                string part = postCondition.substr(startIndex, cutLength);

                // if the part is a variable
                if (_vTable->isVarDeclared(part)) {

                    if (table.hasVar(part)) {
                        string number = to_string(table.getNumber(part));

                        if (endIndex == length - 1 && currentChar != ')') {
                            postCondition.insert(endIndex+1, SSA_DELIMITER + number);
                        }
                        else {
                            postCondition.insert(endIndex, SSA_DELIMITER + number);
                        }

                        endIndex += number.size() + 1;
                    }
                    else {
                        table.declareUndefinedVar(part);

                        if (endIndex == length - 1 && currentChar != ')') {
                            postCondition.insert(endIndex+1, SSA_DELIMITER + to_string(0));
                        }
                        else {
                            postCondition.insert(endIndex, SSA_DELIMITER + to_string(0));
                        }


                        endIndex += 2;
                    }

                }
            }

            startIndex = endIndex + 1;
        }

        endIndex++;
    }

    while (table.hasVarsToDeclare()) {
        auto v = table.popVarToDeclare();
        SMTFile << getVarDeclaration(getSSAName(v.first, v.second), _vTable->getVarType(v.first));
        SMTFile << endl;
    }

    SMTFile << getAssert(getNotFormula(postCondition)) << endl;

    SMTFile << checkSat();

    string result = exec(getCommand(SMTFile.str()));
  
    if (result.substr(0, 6) == "(error") {
        assert(false && "Error with the theorem prover!\n");
    }

    if (result.at(0) == 'u')
        return true;

    return false;
}

string SMTSolverBase::exec(const string &command) const {
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

Interpolants SMTSolverBase::extractInterpolants(const string &line) const {
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

void SMTSolverBase::removeSSANumberingPortion(string& interpolant) const{
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
            char lastChar = interpolant.at(endIndex-1);
            if (isalpha(lastChar) || isdigit(lastChar) || lastChar == '_') {
                ssaPortionFound = true;
                startIndex = endIndex;
            }

        }
        else if (ssaPortionFound && (!isdigit(currentChar) || endIndex == length - 1)) {
            int cutLength = endIndex - startIndex;

            if (endIndex == length - 1 && isdigit(currentChar)) {
                cutLength ++;
            }

            interpolant.erase(startIndex, cutLength);

            endIndex = startIndex - 1;
            ssaPortionFound = false;
        }

        endIndex++;
    }
}