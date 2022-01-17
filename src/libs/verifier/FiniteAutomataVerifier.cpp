#include "FiniteAutomataVerifier.h"
#include "ProofAutomata.h"
#include "Timer.h"
#include "Log.h"
#include "Config.h"
#include <iostream>
#include <queue>
#include <algorithm>

using namespace weaver;
using namespace std;

extern Logger logger;
extern Config config;

bool FiniteAutomataVerifier::verify() {
    logger.info("Start Verifying with Finite Automata...");

    bool correct = false;

    NFA* cfg = &_program->getCFG();

    Timer timer;

    while (true) {
        logger.info("\n");
        logger.info(getRefinementRoundHeader());
        logger.info(getProofSizeHeader());
        
        logger.info(getDeterProofHeader());
        timer.start();        
        DFA* Dproof = _proof.NFAToDFA(_program->getAlphabet());
        _proofDeterTime += timer.stop();

        logger.info(getErrorTraceHeader());
        timer.start();
        Trace errorTrace = proofCheck(cfg, Dproof);
        _proofCheckingTime += timer.stop();

        delete Dproof;

        if (errorTrace.empty()) { // the program is correct
            logger.info("\n");
            logger.info(getVerificationSucessData());
            correct = true;
            break;
        }

        _refinementRound++;

        logger.info(getErrorTraceString(errorTrace));

        logger.info(getGenInterpolHeader());
        
        timer.start();
        Interpolants interpolants = _interpolSolver->generateInterpols(errorTrace);
        _interpolGenTime += timer.stop();

        if (interpolants.empty()) {
            logger.info("\n");
            logger.warn(getVerificationFailureInfo(errorTrace));
            abort();
        }

        logger.info(getInterpolantsString(interpolants));

        logger.info(getGrowProofHeader());
        timer.start();
        _proof.extend(interpolants, _program->getAlphabet());
        _proofExtendTime += timer.stop();
    }

    return correct;
}

Trace FiniteAutomataVerifier::proofCheck(NFA* cfg, DFA* proof) {
    // verify the inclusion on the fly
    set<pair<uint32_t, uint32_t>> states;

    auto startState = make_pair(cfg->getStartState(), proof->getStartState());
    Trace errorTrace;
    bool errorTraceFound = false;

    proofCheckHelper(cfg, proof, states, startState, {}, errorTraceFound, errorTrace);

    return errorTrace;
}

void FiniteAutomataVerifier::proofCheckHelper(NFA* cfg, DFA* proof,
                                                set<pair<uint32_t, uint32_t>>& states,
                                                pair<uint32_t, uint32_t> currentState,
                                                Trace currentTrace,
                                                bool& errorTraceFound,
                                                Trace& errorTrace) {
    if (errorTraceFound == true)
        return;

    states.insert(currentState);

    if (cfg->isAcceptState(currentState.first) && !proof->isAcceptState(currentState.second)) {
        errorTraceFound = true;
        errorTrace.insert(errorTrace.end(), currentTrace.begin(), currentTrace.end());
        return;
    }

    if (cfg->hasTransitionFrom(currentState.first)) {
        for (const auto& t : cfg->getTransitions(currentState.first)) {
            Statement* stmt = t.first;

            for (const auto& nextState1 : t.second) {
                uint32_t nextState2;

                if (stmt == nullptr) {
                    nextState2 = currentState.second;
                }
                else {
                    nextState2 = proof->getTargetState(currentState.second, stmt);
                }

                auto nextState = make_pair(nextState1, nextState2);

                if (states.find(nextState) == states.end()) {
                    if (stmt)
                        currentTrace.push_back(stmt);

                    proofCheckHelper(cfg, proof, states, nextState, currentTrace, errorTraceFound, errorTrace);
                    
                    if (stmt)
                        currentTrace.pop_back();
                }
            }
        }
    }

}
