#include "VerifierBase.h"
#include "Config.h"
#include "SMTInterpol.h"
#include "MathSAT.h"
#include <sstream>

using namespace weaver;

extern Config config;

VerifierBase::VerifierBase(Program* program) :
        _program(program),
        _proof(program->getAlphabet(), &program->getVariableTable()),
        _refinementRound(1),
        _proofCheckingTime(0),
        _proofDeterTime(0),
        _proofExtendTime(0),
        _interpolGenTime(0)
    {
        if (config.interpolantSMTSolver == SMTSolverType::smtinterpol) {
            _interpolSolver = new SMTInterpol(&program->getVariableTable());
        }
        else {
            _interpolSolver = new MathSAT(&program->getVariableTable());
        }

    }


string VerifierBase::getVerificationSucessData() {
    stringstream ss;
    ss << "***********************************************" << endl;
    ss << "**   End: The program is verified correct!   **" << endl;
    ss << "***********************************************" << endl;
    ss << "Total refinement rounds: " + to_string(_refinementRound) << endl;
    ss << "Total size of the proof: " + to_string(_proof.getNumStates()) << endl;
    ss << "Total time to determinize the proof: " + to_string(_proofDeterTime) << endl;
    ss << "Total time to generate interpolants: " + to_string(_interpolGenTime) << endl;
    ss << "Total time to extend the proof: " + to_string(_proofExtendTime) << endl;
    ss << "Total proof construction time: " + to_string(_proofExtendTime + _proofDeterTime + _interpolGenTime) << endl;
    ss << "Total time to do the proof check: " + to_string(_proofCheckingTime) << endl;
    return ss.str();
}

string VerifierBase::getVerificationFailureInfo(const Trace& trace) {
    stringstream ss;
    ss << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
    ss << "xx   Error: The program is incorrect!        xx" << endl;
    ss << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;

    ss << "A valid counterexample trace: " << endl;
    for (const auto& t : trace) {
        ss << t->toString() << endl;
    }

    return ss.str();
}
