#pragma once
#include "Program.h"
#include "SMTSolverBase.h"
#include "ProofAutomata.h"

namespace weaver
{
    class VerifierBase {
    public:
        VerifierBase(Program* program);
        ~VerifierBase() { delete _interpolSolver; };

        virtual bool verify()=0;
    protected:
        string getVerificationSucessData();
        string getVerificationFailureInfo(const Trace& trace);
        string getRefinementRoundHeader() { return "=== Refinement Round: " + to_string(_refinementRound) + " ==="; }
        string getProofSizeHeader() { return "@Current Proof Size: " +  to_string(_proof.getNumStates()); }
        string getDeterProofHeader() { return "@Determinizing the Proof..."; }
        string getErrorTraceHeader() { return "@Getting an Error Trace..."; }
        string getGenInterpolHeader() { return "@Generating Interpolatns..."; }
        string getGrowProofHeader() { return "@Grow the Proof..."; }
        string getErrorTraceString(const Trace& trace) {
            stringstream ss;
            ss << "- Error Trace: " << endl;
            for (const auto& t : trace) {
                ss << t->toString() << endl;
            }
            return ss.str();
        }

        string getInterpolantsString(const Interpolants& interpols) {
            stringstream ss;
            ss << "- Interpolants: " << endl;
            for (const auto& i : interpols) {
                ss << i << endl;
            }

            return ss.str();
        }

        Program* _program;
        SMTSolverBase* _interpolSolver;
        ProofAutomata _proof;
        uint16_t _refinementRound;

        double _proofCheckingTime; // total time spending on proof checking
        double _proofDeterTime; // total time determinizing the proof NFA into DFA
        double _proofExtendTime; // total time growing the proof given new assertions
        double _interpolGenTime; // total time generating the interpolants
    };
} 
