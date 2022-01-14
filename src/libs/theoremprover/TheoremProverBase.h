// #pragma once

// #include "VariableTable.h"
// #include <string>
// #include <queue>
// #include <unordered_set>
// #include "AST.h"
// #include <sstream>
// #include "Statement.h"

// #define SSA_DELIMITER '-'
// #define FORMULA_LABEL "L"

// using namespace std;

// namespace weaver {
//     class Program;

//     /**
//      * Program trace
//      */
//     typedef vector<Statement*> Trace;

//     /**
//      * Interpolants from Sequence Craig Interpolation
//      */
//     typedef vector<string> Interpolants;

//     /**
//      * The single static variable numbering table for generating SMT formulas
//      */
//     class SSANumberingTable {
//     public:
//         uint16_t getVersionNumber(const string& varName);
//         bool isInitializedVarDeclared(const string& varName) { return _declaredInitializedVars.find(varName) != _declaredInitializedVars.end(); }
//         Statement* getInitializationStatement(const string& varName);

//         void initializeVar(const string& varName, Statement* stmt) { _declaredInitializedVars[varName] = stmt; }

//         void clearAllDeclaredVars();

//         bool inNumberTable(const string& varName) { return _numberTable.find(varName) != _numberTable.end(); }

//         void declareNotInitializedVar(const string& varName);

//         bool isUninitializedVarDeclared(const string& varName) { return _declaredUninitializedVars.find(varName) != _declaredUninitializedVars.end(); };
//         void declareInitializedVar(const string& varName, Statement* stmt);

//         pair<string, uint16_t> popInitializedVar();
//         pair<string, uint16_t> popNotInitializedVar();

//         bool isInitializedVarQueueEmpty() { return _varsInitialized.empty(); }
//         bool isNotInitializedVarQueueEmpty() { return _varsNotInitialized.empty(); }
//     private:
//         void enqueueVarInitialized(const pair<string, uint16_t>& var) { _varsInitialized.push(var); }
//         void enqueueVarNotInitialized(const pair<string, uint16_t>& var) { _varsNotInitialized.push(var); }

//         unordered_map<string, uint16_t> _numberTable;

//         // This holds the SSA variables resulting from assignments (initialized)
//         queue<pair<string, uint16_t>> _varsInitialized;

//         // This hold the SSA variables that are not initialized
//         // They are referenced without a prior initialization
//         queue<pair<string, uint16_t>> _varsNotInitialized;

//         // Holds initialized vars have already declared
//         // <var, initialization statement>
//         unordered_map<string, Statement*> _declaredInitializedVars;

//         unordered_set<string> _declaredUninitializedVars;
//     };

//     /**
//      * The interface for all theorem provers for doing craig interpolations.
//      */
//     class TheoremProverBase {
//     public:
//         TheoremProverBase(Program* program);


//         /**
//          * Do craig interpolation on a sequence of program statements (trace)
//          * @param trace
//          * @return A sequence of Interpoolants
//          */
//         virtual Interpolants generateInterpols(const Trace& trace) const;

//         /**
//          *
//          * @param formula1
//          * @param formula2
//          * @return If formula 1 entails formula 2
//          */
//         virtual bool entails(const string &formula1, const string &formula2) const;

//         /**
//          * Check whether two statements are independent of each other
//          * i.e. whether they commute
//          * 
//          * @param s1
//          * @param s2
//          * @return
//          */
//         virtual bool checkIndependenceRelation(Statement* s1, Statement* s2) const;

//         /**
//          * Check the validity of a hoare tripe
//          * @param pre precondition
//          * @param statement
//          * @param post postcondition
//          * @return
//          */
//         virtual bool checkHoareTripe(const string& pre, Statement* statement, const string& post) const;

//     protected:
//         virtual string setInterpolationOptions() const { assert(false && "Not implemented!"); }
//         virtual Interpolants processInterpolationResult(const string& result) const { assert(false && "Not implemented!"); }

//         virtual string setEntailmentOptions() const;
//         virtual string setCheckHoareTripeOptions() const;
//         virtual string setCheckIndependenceOptions() const;

//         virtual string getSSAVarName(const string& varName, uint16_t i) const
//         {return varName + SSA_DELIMITER + to_string(i); }

//         virtual string getVarDeclaration(const string &varName, DataType dType) const;
//         virtual string getFormula(ASTNode* expr,
//                                   Statement* stmt,
//                                   SSANumberingTable& ssaNumberingTable,
//                                   bool isLeftValue=false,
//                                   bool insideAssume=false) const;

//         virtual inline string parenEnclose(const string& s) const
//         { return "(" + s + ")"; }

//         virtual inline string setLogic(const string& logic) const
//         { return parenEnclose("set-logic " + logic); }

//         virtual inline string labelFormula(const string& formula, const string& label) const
//         { return parenEnclose(string("!") + " " + formula + " " + ":named" + " " + label);}

//         virtual inline string setOption(const string& option, const string& value) const
//         { return parenEnclose("set-option " + string(":") + option + " " + value); }

//         virtual inline string getAssert(const string& formula) const
//         { return parenEnclose("assert " + formula ); }

//         virtual inline string getAndFormula(const string& formula1, const string& formula2) const
//         { return parenEnclose("and " + formula1 + " " + formula2 ); }

//         virtual inline string getOrFormula(const string& formula1, const string& formula2) const
//         { return parenEnclose("or " + formula1 + " " + formula2 ); }

//         virtual inline string getAddFormula(const string& formula1, const string& formula2) const
//         { return parenEnclose("+ " + formula1 + " " + formula2 ); }

//         virtual inline string getMinusFormula(const string& formula1, const string& formula2) const
//         { return parenEnclose("- " + formula1 + " " + formula2 ); }

//         virtual inline string getMultFormula(const string& formula1, const string& formula2) const
//         { return parenEnclose("* " + formula1 + " " + formula2 ); }

//         virtual inline string getEqFormula(const string& formula1, const string& formula2) const
//         { return parenEnclose("= " + formula1 + " " + formula2 ); }

//         virtual inline string getNeqFormula(const string& formula1, const string& formula2) const
//         { return parenEnclose("distinct " + formula1 + " " + formula2 ); }

//         virtual inline string getLTFormula(const string& formula1, const string& formula2) const
//         { return parenEnclose("< " + formula1 + " " + formula2 ); }

//         virtual inline string getLTEFormula(const string& formula1, const string& formula2) const
//         { return parenEnclose("<= " + formula1 + " " + formula2 ); }

//         virtual inline string getGTFormula(const string& formula1, const string& formula2) const
//         { return parenEnclose("> " + formula1 + " " + formula2 ); }

//         virtual inline string getGTEFormula(const string& formula1, const string& formula2) const
//         { return parenEnclose(">= " + formula1 + " " + formula2 ); }

//         virtual inline string getNotFormula(const string& formula) const
//         { return parenEnclose("not " + formula ); }

//         virtual inline string getNegationFormula(const string& formula) const
//         { return parenEnclose("- " + formula ); };

//         virtual inline string getImplyFormula(const string& formula1, const string& formula2) const
//         { return parenEnclose("=> " + formula1 + " " + formula2); }

//         virtual inline string getTrue() const { return "true"; }
//         virtual inline string getFalse() const { return "false"; }
//         virtual inline string checkSat() const { return parenEnclose("check-sat"); }
//         virtual inline string getInterpolants(const vector<string>& labels) const
//         {
//             stringstream ss;
//             for (const auto& l: labels) {
//                 ss << l << " ";
//             }

//             return parenEnclose("get-interpolants " + ss.str());
//         }

//         virtual inline string getCommand(const string& SMTFile) const =0;

//         // we do not want the interpolants have ssa numberings. So remove them
//         virtual void removeSSANumberingPortion(string& interpolant) const;

//         virtual Interpolants extractInterpolants(const string& line) const;

//         // This is for executing an arbitrary command on the system
//         string exec(const string &command) const;

//         Program* _program;
//         VariableTable* _vTable;
//     };
// }


