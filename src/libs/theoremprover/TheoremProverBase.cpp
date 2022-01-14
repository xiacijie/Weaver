// #include "TheoremProverBase.h"
// #include <cstdio>
// #include <memory>
// #include "Program.h"

// #define BUFFER_SIZE 128

// using namespace std;
// using namespace weaver;

// TheoremProverBase::TheoremProverBase(Program* program) :
//     _program(program),
//     _vTable(&program->getVariableTable())
// {}

// string TheoremProverBase::setEntailmentOptions() const {
//     stringstream ss;
//     ss << setOption("print-success", getFalse());
//     ss << setLogic("QF_AUFLIA");
//     ss << endl;
//     return ss.str();
// }

// string TheoremProverBase::setCheckHoareTripeOptions() const {
//     stringstream ss;
//     ss << setOption("print-success", getFalse());
//     ss << setLogic("QF_AUFLIA");
//     ss << endl;
//     return ss.str();
// }

// string TheoremProverBase::setCheckIndependenceOptions() const {
//     stringstream ss;
//     ss << setOption("print-success", getFalse());
//     ss << setLogic("QF_UFLIA");
//     ss << endl;
//     return ss.str();
// }

// Interpolants TheoremProverBase::generateInterpols(const Trace &trace) const {
//     SSANumberingTable ssaTable;

//     stringstream SMTFile;

//     SMTFile << setInterpolationOptions();

//     for (int i = 0; i < trace.size(); i ++) {
//         Statement* stmt = trace[i];
//         ASTNode* node = stmt->getNode();

//         string formula = getFormula(node, stmt, ssaTable);
//         while (!ssaTable.isNotInitializedVarQueueEmpty()) {
//             auto var = ssaTable.popNotInitializedVar();
//             string varDecl = getVarDeclaration(getSSAVarName(var.first, var.second), _vTable->getVarType(var.first));
//             SMTFile << varDecl;
//             SMTFile << endl;
//         }

//         // need to declare those vars
//         while (!ssaTable.isInitializedVarQueueEmpty()) {
//             auto newSSAVar = ssaTable.popInitializedVar();
//             string ssaVarName = getSSAVarName(newSSAVar.first, newSSAVar.second);
//             string varDecl = getVarDeclaration(ssaVarName, _vTable->getVarType(newSSAVar.first));
//             SMTFile << varDecl;
//             SMTFile << endl;
//         }

//         SMTFile << getAssert(labelFormula(formula, FORMULA_LABEL + to_string(i)));
//         SMTFile << endl;

//     }

//     SMTFile << checkSat();
//     SMTFile << endl;

//     vector<string> labels;
//     labels.reserve(trace.size());
//     for (int i = 0; i < trace.size(); i ++) {
//         labels.push_back(FORMULA_LABEL + to_string(i));
//     }
//     // cout << SMTFile.str() << endl;

//     SMTFile << getInterpolants(labels) << endl;
    
//     string result = exec(getCommand(SMTFile.str()));
//     // cout << result << endl;
//     return processInterpolationResult(result);
// }

// bool TheoremProverBase::entails(const string &formula1, const string &formula2) const {
//     stringstream SMTFile;

//     SMTFile << setEntailmentOptions();

//     for (auto v : _vTable->getGlobalVariables()) {
//         SMTFile << getVarDeclaration(v.first, v.second) << endl;
//     }

//     SMTFile << getAssert(getNotFormula(getImplyFormula(formula1, formula2)));
//     SMTFile << endl;
//     SMTFile << checkSat();

//     string result = exec(getCommand(SMTFile.str()));

//     if (result.substr(0, 6) == "(error") {
//         assert(false && "Error with the theorem prover!\n");
//     }

//     if (result.at(0) == 'u')
//         return true;

//     return false;
// }

// bool TheoremProverBase::checkIndependenceRelation(Statement *s1, Statement *s2) const {
//     if (s1->getThreadID() == 0 || s2->getThreadID() == 0) {
//         return false;
//     }

//     if (s1->getThreadID() == s2->getThreadID()) { // statements in the same thread are dependent anyway
//         return false;
//     }

//     if (s1->getNode()->isBool() || s2->getNode()->isBool()) { // we do not handle assertions now
//         return false;
//     }

//     stringstream SMTFile;
//     SMTFile << setCheckIndependenceOptions();

//     SSANumberingTable ssaNumberingTable;

//     // generate the SMT formulas for s1s2
//     string s1Formula1 = getFormula(s1->getNode(), s1, ssaNumberingTable);
//     string s2Formula1 = getFormula(s2->getNode(), s2, ssaNumberingTable);

//     vector<pair<string, uint16_t>> varsNotInitialized1;
//     while (!ssaNumberingTable.isNotInitializedVarQueueEmpty()) {
//         auto varNotInit = ssaNumberingTable.popNotInitializedVar();
//         SMTFile << getVarDeclaration(getSSAVarName(varNotInit.first,varNotInit.second), _vTable->getVarType(varNotInit.first));
//         SMTFile << endl;
//         varsNotInitialized1.push_back(varNotInit);
//     }

//     unordered_map<string, uint16_t> largestVarNumber1;
//     while (!ssaNumberingTable.isInitializedVarQueueEmpty()) {
//         pair<string, uint16_t> varInit = ssaNumberingTable.popInitializedVar();
//         SMTFile << getVarDeclaration(getSSAVarName(varInit.first, varInit.second), _vTable->getVarType(varInit.first));
//         SMTFile << endl;

//         if (largestVarNumber1.find(varInit.first) == largestVarNumber1.end()) {
//             largestVarNumber1[varInit.first] = varInit.second;
//         }
//         else {
//             largestVarNumber1[varInit.first] = max(largestVarNumber1[varInit.first], varInit.second);
//         }
//     }

//     // let all vars uninitialized but keep the ssa numberings
//     ssaNumberingTable.clearAllDeclaredVars();

//     // generate the SMT formulas for s2s1
//     string s2Formula2 = getFormula(s2->getNode(), s2, ssaNumberingTable);
//     string s1Formula2 = getFormula(s1->getNode(), s1, ssaNumberingTable);

//     vector<pair<string, uint16_t>> varsNotInitialized2;
//     while (!ssaNumberingTable.isNotInitializedVarQueueEmpty()) {
//         auto varNotInit = ssaNumberingTable.popNotInitializedVar();
//         SMTFile << getVarDeclaration(getSSAVarName(varNotInit.first,varNotInit.second), _vTable->getVarType(varNotInit.first));
//         SMTFile << endl;
//         varsNotInitialized2.push_back(varNotInit);
//     }

//     unordered_map<string, uint16_t> largestVarNumber2;
//     while (!ssaNumberingTable.isInitializedVarQueueEmpty()) {
//         pair<string, uint16_t> varInit = ssaNumberingTable.popInitializedVar();
//         SMTFile << getVarDeclaration(getSSAVarName(varInit.first, varInit.second), _vTable->getVarType(varInit.first));
//         SMTFile << endl;
//         if (largestVarNumber2.find(varInit.first) == largestVarNumber2.end()) {
//             largestVarNumber2[varInit.first] = varInit.second;
//         }
//         else {
//             largestVarNumber2[varInit.first] = max(largestVarNumber2[varInit.first], varInit.second);
//         }
//     }

//     SMTFile << getAssert(getAndFormula(s1Formula1, s2Formula1)) << endl;
//     SMTFile << getAssert(getAndFormula(s2Formula2, s1Formula2)) << endl;

//     for (const auto & v1 : varsNotInitialized1) {
//         for (const auto& v2 : varsNotInitialized2) {
//             if (v1.first == v2.first) {
//                 SMTFile << getAssert(getEqFormula(getSSAVarName(v1.first, v1.second), getSSAVarName(v2.first, v2.second)));
//                 SMTFile << endl;
//             }
//         }
//     }

//     string conjunctEQ;
//     for (const auto& it : largestVarNumber1) {
//         string varName = it.first;

//         if (conjunctEQ.empty()) {
//             conjunctEQ = getEqFormula(
//                     getSSAVarName(varName, largestVarNumber1[varName]),
//                     getSSAVarName(varName, largestVarNumber2[varName]));
//         }
//         else {
//             conjunctEQ = getAndFormula(conjunctEQ,
//                                        getEqFormula(
//                                                getSSAVarName(varName, largestVarNumber1[varName]),
//                                                getSSAVarName(varName, largestVarNumber2[varName])
//                                        ));
//         }
//     }

//     SMTFile << getAssert(getNotFormula(conjunctEQ))  << endl;

//     SMTFile << checkSat() << endl;

//     string result = exec(getCommand(SMTFile.str()));

//     if (result.at(0) =='u')
//         return true;

//     return false;
// }

// bool TheoremProverBase::checkHoareTripe(const string &pre, Statement *statement, const string &post) const {
//     // pre-condition is false, then always valid
//     if (pre == getFalse()) {
//         return true;
//     }

//     // post-condition is true, then always valid
//     if (post == getTrue()) {
//         return true;
//     }

//     SSANumberingTable table;

//     stringstream SMTFile;
//     SMTFile << setCheckHoareTripeOptions();
//     SMTFile << endl;

//     string preCondition = pre;
//     string postCondition = post;

//     // update the ssa numbering of the pre-condition assertion
//     int startIndex = 0;
//     int endIndex = 0;

//     while (true) {
//         int length = preCondition.size();
//         if (endIndex >= length) {
//             break;
//         }

//         char currentChar = preCondition.at(endIndex);
//         if (currentChar == ' ' || currentChar == '(' || currentChar == ')' || endIndex == length - 1) {
//             if (endIndex >= startIndex) {
//                 int cutLength = endIndex - startIndex;

//                 if (endIndex == length - 1 && currentChar != ')') {
//                     cutLength ++;
//                 }

//                 string part = preCondition.substr(startIndex, cutLength);

//                 // if the part is a variable
//                 if (_vTable->isVarDeclared(part)) {
//                     if (!table.isUninitializedVarDeclared(part)) {
//                         table.declareNotInitializedVar(part);
//                     }

//                     if (endIndex == length - 1 && currentChar != ')') {
//                         preCondition.insert(endIndex+1, SSA_DELIMITER + to_string(0));
//                     }
//                     else {
//                         preCondition.insert(endIndex, SSA_DELIMITER + to_string(0));
//                     }

//                     endIndex += 2;
//                 }
//             }

//             startIndex = endIndex + 1;
//         }

//         endIndex++;
//     }

//     while(!table.isNotInitializedVarQueueEmpty()) {
//         auto decl = table.popNotInitializedVar();
//         SMTFile << getVarDeclaration(
//                 getSSAVarName(decl.first, decl.second),
//                 _vTable->getVarType(decl.first))
//                 << endl;

//     }

//     SMTFile << getAssert(preCondition) << endl;

//     string formula;

//     // get the SMT formula for the statement
//     if (statement == nullptr) {
//         formula = getTrue();
//     }
//     else {
//         formula = getFormula(statement->getNode(), statement, table);
//     }


//     while(!table.isNotInitializedVarQueueEmpty()) {
//         auto decl = table.popNotInitializedVar();
//         SMTFile << getVarDeclaration(
//                 getSSAVarName(decl.first, decl.second),
//                 _vTable->getVarType(decl.first))
//                 << endl;

//     }

//     while(!table.isInitializedVarQueueEmpty()) {
//         auto decl = table.popInitializedVar();
//         SMTFile << getVarDeclaration(
//                 getSSAVarName(decl.first, decl.second),
//                 _vTable->getVarType(decl.first))
//                 << endl;

//     }

//     SMTFile << getAssert(formula) << endl;

//     startIndex = 0;
//     endIndex = 0;
//     while (true) {
//         int length = postCondition.size();
//         if (endIndex >= length) {
//             break;
//         }

//         char currentChar = postCondition.at(endIndex);
//         if (currentChar == ' ' || currentChar == '(' || currentChar == ')' || endIndex == length - 1) {
//             if (endIndex >= startIndex) {
//                 int cutLength = endIndex - startIndex;

//                 if (endIndex == length - 1 && currentChar != ')') {
//                     cutLength++;
//                 }

//                 string part = postCondition.substr(startIndex, cutLength);

//                 // if the part is a variable
//                 if (_vTable->isVarDeclared(part)) {

//                     if (table.isInitializedVarDeclared(part)) {
//                         string number = to_string(table.getVersionNumber(part));

//                         if (endIndex == length - 1 && currentChar != ')') {
//                             postCondition.insert(endIndex+1, SSA_DELIMITER + number);
//                         }
//                         else {
//                             postCondition.insert(endIndex, SSA_DELIMITER + number);
//                         }

//                         endIndex += number.size() + 1;
//                     }
//                     else {
//                         if (!table.isUninitializedVarDeclared(part)) {
//                             table.declareNotInitializedVar(part);
//                         }

//                         if (endIndex == length - 1 && currentChar != ')') {
//                             postCondition.insert(endIndex+1, SSA_DELIMITER + to_string(0));
//                         }
//                         else {
//                             postCondition.insert(endIndex, SSA_DELIMITER + to_string(0));
//                         }


//                         endIndex += 2;
//                     }

//                 }
//             }

//             startIndex = endIndex + 1;
//         }

//         endIndex++;
//     }

//     while(!table.isNotInitializedVarQueueEmpty()) {
//         auto decl = table.popNotInitializedVar();
//         SMTFile << getVarDeclaration(
//                 getSSAVarName(decl.first, decl.second),
//                 _vTable->getVarType(decl.first))
//                 << endl;

//     }

//     SMTFile << getAssert(getNotFormula(postCondition)) << endl;

//     SMTFile << checkSat();

//     string result = exec(getCommand(SMTFile.str()));
  
//     if (result.substr(0, 6) == "(error") {
//         assert(false && "Error with the theorem prover!\n");
//     }

//     if (result.at(0) == 'u')
//         return true;

//     return false;
// }

// void SSANumberingTable::declareNotInitializedVar(const string &varName) {
//     if (!inNumberTable(varName)) {
//         _numberTable[varName] = 0;
//     }
//     else {
//         _numberTable[varName]++;
//     }

//     enqueueVarNotInitialized(make_pair(varName, _numberTable[varName]));
//     _declaredUninitializedVars.insert(varName);
// }

// uint16_t SSANumberingTable::getVersionNumber(const string &varName) {
//     if (!inNumberTable(varName))
//         return 0;

//     return _numberTable[varName];
// }

// void SSANumberingTable::declareInitializedVar(const string &varName,Statement* stmt) {
//     if (!inNumberTable(varName)) {
//         _numberTable[varName] = 0;
//     }
//     else {
//         _numberTable[varName]++;
//     }

//     enqueueVarInitialized(make_pair(varName, _numberTable[varName]));
//     initializeVar(varName, stmt);
// }

// Statement* SSANumberingTable::getInitializationStatement(const string &varName) {
//     const auto& it = _declaredInitializedVars.find(varName);
//     assert(it != _declaredInitializedVars.end() && "Error");

//     return it->second;
// }

// pair<string, uint16_t> SSANumberingTable::popInitializedVar() {
//     assert(!isInitializedVarQueueEmpty() && "Error");

//     auto newVar = _varsInitialized.front();
//     _varsInitialized.pop();

//     return newVar;
// }

// pair<string, uint16_t> SSANumberingTable::popNotInitializedVar() {
//     assert(!isNotInitializedVarQueueEmpty() && "Empty");
//     auto var = _varsNotInitialized.front();
//     _varsNotInitialized.pop();

//     return var;
// }

// void SSANumberingTable::clearAllDeclaredVars() {
//     _declaredInitializedVars.clear();
//     _declaredUninitializedVars.clear();
// }

// string TheoremProverBase::exec(const string &command) const {
//     shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
//     if (!pipe)
//         assert(false && "Pipe failed to open!\n");

//     char buffer[BUFFER_SIZE];

//     string result = "";
//     while (!feof(pipe.get())) {
//         if (fgets(buffer, BUFFER_SIZE, pipe.get()) != NULL)
//             result += buffer;
//     }
//     return result;
// }

// string TheoremProverBase::getVarDeclaration(const string &varName, DataType dType) const {
//     stringstream ss;
//     ss << "declare-const" << " " << varName << " ";
//     switch (dType) {
//         case DataType::Int: ss << "Int"; break;
//         case DataType::Bool: ss << "Bool"; break;
//         case DataType::IntArray: ss << "(Array Int Int)"; break;
//         default:
//             assert(false && "Error in getVarDeclaration!\n");
//             break;
//     }
//     string decl = parenEnclose(ss.str());
//     return decl;
// }

// string TheoremProverBase::getFormula(ASTNode *node, Statement* stmt, SSANumberingTable& ssaNumberingTable, bool isLeftValue, bool insideAssume) const {
//     // Left value is the left-hand side of an assignment statement
//     // Need to update the ssa numbering table
//     if (isLeftValue) {
//         string varName = node->getIdName();
//         ssaNumberingTable.declareInitializedVar(varName, stmt);
//     }

//     if (node->isNoAction()) {
//         return getTrue();
//     }

//     if (node->isAtomic()) {
//         if (node->getNumChildren() == 1) {
//             return getFormula(node->getChild(0), stmt, ssaNumberingTable);
//         }

//         vector<string> statementFormulas;
//         statementFormulas.reserve(node->getNumChildren());
//         for (int i = 0; i < node->getNumChildren(); i ++) {
//             ASTNode* child = node->getChild(i);
//             statementFormulas.push_back(getFormula(child, stmt, ssaNumberingTable));
//         }

//         string conjunctStatements = getAndFormula(statementFormulas[0], statementFormulas[1]);
//         for (int i = 2; i < statementFormulas.size(); i ++) {
//             conjunctStatements = getAndFormula(conjunctStatements, statementFormulas[i]);
//         }

//         return conjunctStatements;
//     }

//     if (node->isAssume()) {
//         return getFormula(node->getChild(0), stmt, ssaNumberingTable, false, true);
//     }

//     if (node->isId()) { // get the ssa version of this variable
//         string varName = node->getIdName();

//         if (!ssaNumberingTable.isInitializedVarDeclared(varName) && !ssaNumberingTable.isUninitializedVarDeclared(varName)) { // first time encounter this var and not declared
//             ssaNumberingTable.declareNotInitializedVar(varName); //should be set to 0
//         }

//         string ssaVarName = getSSAVarName(varName, ssaNumberingTable.getVersionNumber(varName));
//         return ssaVarName;
//     }

//     if (node->isIntConst())
//         return to_string(node->getInt());

//     if (node->isBoolConst()) {
//         switch (node->getBool()) {
//             case BooleanValue::True: return getTrue();
//             case BooleanValue::False: return getFalse();
//             case BooleanValue::NonDeter:
//                 assert(false&& "Non deter bool value should not occur here!\n");
//                 return nullptr;
//         }
//     }

//     if (node->getNodeType() == NodeType::Not) {
//         return getNotFormula(getFormula(node->getChild(0), stmt, ssaNumberingTable));
//     }
//     else if (node->getNodeType() == NodeType::Negation) {
//         return getNegationFormula(getFormula(node->getChild(0), stmt, ssaNumberingTable));
//     }
//     else {
//         string left;
//         string right;

//         if (node->isAssign()) {
//             right = getFormula(node->getChild(1), stmt, ssaNumberingTable, false, insideAssume);
//             left = getFormula(node->getChild(0), stmt, ssaNumberingTable, true, insideAssume);
//         }
//         // else if (insideAssume) {
//         //     //either lhs or rhs can be initialized
//         //     if (node->getChild(1)->isId()) { // right
//         //         const string& varName = node->getChild(1)->getIdName();
//         //         if (!ssaNumberingTable.isInitializedVarDeclared(varName) ) {
//         //             right = getFormula(node->getChild(1), stmt, ssaNumberingTable, true, insideAssume);
//         //         }
//         //     }

//         //     if (node->getChild(0)->isId()) { // left
//         //         const string& varName = node->getChild(0)->getIdName();
//         //         if (!ssaNumberingTable.isInitializedVarDeclared(varName) ) {
//         //             left = getFormula(node->getChild(0), stmt, ssaNumberingTable, true, insideAssume);
//         //         }
//         //     }
//         // }

//         if (left.empty()) {
//             left = getFormula(node->getChild(0), stmt, ssaNumberingTable, false, insideAssume);
//         }

//         if (right.empty()) {
//             right = getFormula(node->getChild(1), stmt, ssaNumberingTable, false, insideAssume);
//         }

//         switch (node->getNodeType()) {
//             case NodeType::Addition:
//                 return getAddFormula(left, right);
//             case NodeType::Subtraction:
//                 return getMinusFormula(left, right);
//             case NodeType::Multiplication:
//                 return getMultFormula(left, right);
//             case NodeType::Assignment:
//                 return getEqFormula(left, right);
//             case NodeType::CompareLT:
//                 return getLTFormula(left, right);
//             case NodeType::CompareLTE:
//                 return getLTEFormula(left, right);
//             case NodeType::CompareGT:
//                 return getGTFormula(left, right);
//             case NodeType::CompareGTE:
//                 return getGTEFormula(left, right);
//             case NodeType::CompareEQ:
//                 return getEqFormula(left, right);
//             case NodeType::CompareNEQ:
//                 return getNeqFormula(left, right);
//             case NodeType::And:
//                 return getAndFormula(left, right);
//             case NodeType::Or:
//                 return getOrFormula(left, right);
//             default:
//                 cerr << NodeTypeLabels[node->getNodeType()] << endl;
//                 assert(false && "Error in getFormula!\n");
//         }
//     }
// }

// Interpolants TheoremProverBase::extractInterpolants(const string &line) const {
//     Interpolants interpolants;
//     interpolants.push_back("true");

//     // we need extract the interpolants from the interpolants string
//     int searchStartIndex = 0;
//     int searchEndIndex = 0;
//     // the stack that stores the open and end parens
//     vector<char> parenStack;
//     while (searchEndIndex < line.size()) {
//         while (line.at(searchStartIndex) == ' ')
//             searchStartIndex ++;

//         char currentChar = line.at(searchEndIndex);

//         if (currentChar == '(') {
//             parenStack.push_back('(');
//         }
//         else if (currentChar == ')') {
//             if (parenStack.empty() || parenStack.at(parenStack.size()-1) != '(') {
//                 cerr << "Stack size: " << parenStack.size() << endl;
//                 cerr << "SearchEndIndex: " << searchEndIndex << endl;
//                 assert(false && "Error in popping parenStack!\n");
//             }
//             parenStack.pop_back();

//             if (parenStack.empty()) {
//                 string intpl = line.substr(searchStartIndex, searchEndIndex-searchStartIndex+1);
//                 removeSSANumberingPortion(intpl);
//                 interpolants.push_back(intpl);
//                 searchStartIndex = searchEndIndex + 1;
//             }

//         }
//         else if (currentChar == 't' && parenStack.empty()) {
//             interpolants.push_back(line.substr(searchStartIndex, 4));
//             searchEndIndex += 3;
//             searchStartIndex = searchEndIndex + 1;
//         }
//         else if (currentChar == 'f' && parenStack.empty()) {
//             interpolants.push_back(line.substr(searchStartIndex, 5));
//             searchEndIndex += 4;
//             searchStartIndex = searchEndIndex + 1;
//         }

//         searchEndIndex ++;
//     }

//     interpolants.push_back("false");

//     return interpolants;
// }

// void TheoremProverBase::removeSSANumberingPortion(string& interpolant) const{
//     bool ssaPortionFound = false;
//     int startIndex = 0;
//     int endIndex = 0;

//     while (true) {
//         int length = interpolant.size();

//         if (endIndex >= length) {
//             break;
//         }

//         char currentChar = interpolant.at(endIndex);

//         if (currentChar == SSA_DELIMITER) {
//             // make sure the '-' is not the subtraction
//             char lastChar = interpolant.at(endIndex-1);
//             if (isalpha(lastChar) || isdigit(lastChar) || lastChar == '_') {
//                 ssaPortionFound = true;
//                 startIndex = endIndex;
//             }

//         }
//         else if (ssaPortionFound && (!isdigit(currentChar) || endIndex == length - 1)) {
//             int cutLength = endIndex - startIndex;

//             if (endIndex == length - 1 && isdigit(currentChar)) {
//                 cutLength ++;
//             }

//             interpolant.erase(startIndex, cutLength);

//             endIndex = startIndex - 1;
//             ssaPortionFound = false;
//         }

//         endIndex++;
//     }
// }

