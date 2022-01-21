#pragma once

#include "VariableTable.h"
#include "Statement.h"
#include "SSANumberingTable.h"
#include <sstream>
#include <vector>
#include <memory>

#define BUFFER_SIZE 128

namespace weaver {

    /**
     * @brief Program Trace
     * 
     */
    typedef vector<Statement*> Trace;

    /**
     * @brief Interpolants from Sequence Craig Interpolation
     * 
     */
    typedef vector<string> Interpolants;


    class SMTSolverBase {
    public:
        SMTSolverBase(VariableTable* vTable) :
            _vTable(vTable)
        {}

        /**
         * Do craig interpolation on a sequence of program statements (trace)
         * @param trace
         * @return A sequence of Interpoolants
         */
        Interpolants generateInterpols(const Trace& trace) const;

        /**
         * Check whether two statements are independent of each other
         * i.e. whether they commute
         * 
         * @param s1
         * @param s2
         * @return
         */
        bool checkIndependenceRelation(Statement* s1, Statement* s2) const;

        /**
         * Check the validity of a hoare tripe
         * @param pre precondition
         * @param statement
         * @param post postcondition
         * @return
         */
        bool checkHoareTripe(const string& pre, Statement* statement, const string& post) const;
    protected:
        virtual string setInterpolationOptions() const { assert(false && "Not implemented!"); }
        virtual string setCheckHoareTripeOptions() const;
        virtual string setCheckIndependenceOptions() const;

        virtual Interpolants processInterpolationResult(const string& result) const { assert(false && "Not implemented!"); }

        virtual string getVarDeclaration(const string &varName, DataType dType) const;
        virtual string getFormula(ASTNode* node,
                                  SSANumberingTable& ssaNumberingTable,
                                  bool isLeftValue=false) const;

        virtual inline string getSSAName(const string& varName, uint16_t i) const 
        { return varName + SSA_DELIMITER + to_string(i); }

        virtual inline string parenEnclose(const string& s) const
        { return "(" + s + ")"; }

        virtual inline string setLogic(const string& logic) const
        { return parenEnclose("set-logic " + logic); }

        virtual inline string labelFormula(const string& formula, const string& label) const
        { return parenEnclose(string("!") + " " + formula + " " + ":named" + " " + label);}

        virtual inline string setOption(const string& option, const string& value) const
        { return parenEnclose("set-option " + string(":") + option + " " + value); }

        virtual inline string getAssert(const string& formula) const
        { return parenEnclose("assert " + formula ); }

        virtual inline string getAndFormula(const string& formula1, const string& formula2) const
        { return parenEnclose("and " + formula1 + " " + formula2 ); }

        virtual inline string getOrFormula(const string& formula1, const string& formula2) const
        { return parenEnclose("or " + formula1 + " " + formula2 ); }

        virtual inline string getAddFormula(const string& formula1, const string& formula2) const
        { return parenEnclose("+ " + formula1 + " " + formula2 ); }

        virtual inline string getMinusFormula(const string& formula1, const string& formula2) const
        { return parenEnclose("- " + formula1 + " " + formula2 ); }

        virtual inline string getMultFormula(const string& formula1, const string& formula2) const
        { return parenEnclose("* " + formula1 + " " + formula2 ); }

        virtual inline string getEqFormula(const string& formula1, const string& formula2) const
        { return parenEnclose("= " + formula1 + " " + formula2 ); }

        virtual inline string getNeqFormula(const string& formula1, const string& formula2) const
        { return parenEnclose("distinct " + formula1 + " " + formula2 ); }

        virtual inline string getLTFormula(const string& formula1, const string& formula2) const
        { return parenEnclose("< " + formula1 + " " + formula2 ); }

        virtual inline string getLTEFormula(const string& formula1, const string& formula2) const
        { return parenEnclose("<= " + formula1 + " " + formula2 ); }

        virtual inline string getGTFormula(const string& formula1, const string& formula2) const
        { return parenEnclose("> " + formula1 + " " + formula2 ); }

        virtual inline string getGTEFormula(const string& formula1, const string& formula2) const
        { return parenEnclose(">= " + formula1 + " " + formula2 ); }

        virtual inline string getNotFormula(const string& formula) const
        { return parenEnclose("not " + formula ); }

        virtual inline string getNegationFormula(const string& formula) const
        { return parenEnclose("- " + formula ); };

        virtual inline string getImplyFormula(const string& formula1, const string& formula2) const
        { return parenEnclose("=> " + formula1 + " " + formula2); }

        virtual inline string getStoreFormula(const string& array, const string& index, const string& value) const  
        { return getEqFormula(array, parenEnclose("store " + array + " " + index + " " + value)); }

        virtual inline string getSelectFormula(const string& array, const string& index) const  
        { return parenEnclose("select " + array + " " + index); }

        virtual inline string getTrue() const { return "true"; }
        virtual inline string getFalse() const { return "false"; }
        virtual inline string checkSat() const { return parenEnclose("check-sat"); }
        virtual inline string getInterpolants(const vector<string>& labels) const
        {
            stringstream ss;
            for (const auto& l: labels) {
                ss << l << " ";
            }

            return parenEnclose("get-interpolants " + ss.str());
        }

        virtual inline string getCommand(const string& SMTFile) const =0;

        // we do not want the interpolants have ssa numberings. So remove them
        virtual void removeSSANumberingPortion(string& interpolant) const;

        // This is for executing an arbitrary command on the system
        string exec(const string &command) const;


    private:
        VariableTable* _vTable;
    };
}