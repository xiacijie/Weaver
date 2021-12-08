#include "CFGBuilder.h"


using namespace weaver;

uint32_t CFGBuilder::build() {
    if (_threadID ==  0) { // building the CFG for the whole program
        _cfg->addState(0);
        _cfg->addState(1);
        _cfg->setStartState(1);
        _cfg->setAcceptState(0); // the error state

        return statementHandleDispatch(_root, _cfg->getStartState());
    }
    else { // build the CFG for a thread
        // current we assume there is no assertion in a thread to build a CFG for
        _cfg->addState(0);
        _cfg->setStartState(0);

        return statementHandleDispatch(_root, _cfg->getStartState());;
    }

}

void CFGBuilder::addTransition(uint32_t fromState, Statement *statement, uint32_t toState) {
    _cfg->addTransition(fromState, statement, toState);

    if (statement)
        _alphabet.insert(statement);
}


uint32_t CFGBuilder::statementHandleDispatch(ASTNode *node, uint32_t currentState) {

    if (node->isProgram() || node->isBlock()) {
        return iterateThroughStatements(node, currentState);
    }

    if (node->isAssign() || node->isAssume() || node->isAssert() || node->isAtomic()) {
        currentState = handleStatementWithNoControlFlow(node, currentState);
    }
    else if (node->isConditional()) {
        if (node->getChild(0)->isNonDeterBool()) {
            currentState = handleNonDeterConditionalStatement(node, currentState);
        }
        else {
            currentState = handleConditionalStatement(node, currentState);
        }
    }
    else if (node->isLoop()) {
        if (node->getChild(0)->isNonDeterBool()) {
            currentState = handleNonDeterLoopStatement(node, currentState);
        }
        else {
            currentState = handleLoopStatement(node, currentState);
        }
    }
    else if (node->isParallel()) {
        currentState = handleParallelStatement(node, currentState);
    }
    else {
        assert(false && "Unhandled Statement!\n");
    }

    return currentState;
}

uint32_t CFGBuilder::iterateThroughStatements(ASTNode *node, uint32_t currentState) {

    for (int i = 0; i < node->getNumChildren(); i ++) {
        ASTNode* child = node->getChild(i);
        currentState = statementHandleDispatch(child, currentState);
    }

    return currentState;
}

uint32_t CFGBuilder::handleStatementWithNoControlFlow(ASTNode *node, uint32_t currentState) {

    if (node->isAssert()) {
        assert(_threadID == 0 && "Currently We do not allow assertion to be declared in a parallel block\n");

        ASTNode* notCondition = ASTNode::create(NodeType::Not, DataType::Bool, _program, {node->getChild(0)});

        addTransition(currentState, createStatement(notCondition), *_cfg->getAcceptStates().begin());
        return currentState;
    }
    else {
        uint32_t targetState = _cfg->getNumStates();

        _cfg->addState(targetState);

        if (node->isAssume()) {
            addTransition(currentState, createStatement(node->getChild(0)), targetState);
        }
        else { // assignment or atomic
            addTransition(currentState, createStatement(node), targetState);
        }

        return targetState;
    }
}

uint32_t CFGBuilder::handleNonDeterConditionalStatement(ASTNode *node, uint32_t currentState) {

    uint32_t ifBranchLastState = statementHandleDispatch(node->getChild(1), currentState);

    if (node->getNumChildren() == 3) { // has an else branch
        uint32_t elseBranchLastState = statementHandleDispatch(node->getChild(2), currentState);

        addTransition(elseBranchLastState, nullptr, ifBranchLastState);

    }
    else { // does not have an else branch
        addTransition(currentState, nullptr, ifBranchLastState);
    }

    return ifBranchLastState;

}

uint32_t CFGBuilder::handleConditionalStatement(ASTNode *node, uint32_t currentState) {

    uint32_t ifTrueBranchStartState = _cfg->getNumStates();
    _cfg->addState(ifTrueBranchStartState);

    // The transition when the condition is true
    addTransition(currentState, createStatement(node->getChild(0)), ifTrueBranchStartState);
    ASTNode* ifTrueBranch = node->getChild(1); //should be a block

    uint32_t ifTrueBranchLastState = statementHandleDispatch(ifTrueBranch, ifTrueBranchStartState);

    // The transition when the condition is false
    if (node->getNumChildren() == 3) { //has an else branch

        uint32_t ifFalseBranchStartState = _cfg->getNumStates();
        _cfg->addState(_cfg->getNumStates());

        ASTNode* notCondition = ASTNode::create(NodeType::Not, DataType::Bool, _program, {node->getChild(0)});

        addTransition(currentState, createStatement(notCondition), ifFalseBranchStartState);

        ASTNode* ifFalseBranch = node->getChild(2);
        uint32_t ifFalseBranchLastState = statementHandleDispatch(ifFalseBranch, ifFalseBranchStartState);

        addTransition(ifFalseBranchLastState, nullptr, ifTrueBranchLastState);
    }
    else { // do not have an else branch
        ASTNode* notCondition = ASTNode::create(NodeType::Not, DataType::Bool, _program, {node->getChild(0)});

        addTransition(currentState, createStatement(notCondition), ifTrueBranchLastState);
    }

    return ifTrueBranchLastState;
}

uint32_t CFGBuilder::handleNonDeterLoopStatement(ASTNode *node, uint32_t currentState) {

    uint32_t loopBodyLastState = statementHandleDispatch(node->getChild(1), currentState);
    addTransition(loopBodyLastState, nullptr, currentState);

    uint32_t afterLoopState = _cfg->getNumStates();
    _cfg->addState(afterLoopState);
    addTransition(currentState, nullptr, afterLoopState);
    return afterLoopState;
}

uint32_t CFGBuilder::handleLoopStatement(ASTNode *node, uint32_t currentState) {

    ASTNode* loopCondition = node->getChild(0);

    uint32_t loopBodyStartState = _cfg->getNumStates();
    _cfg->addState(loopBodyStartState);

    addTransition(currentState, createStatement(loopCondition), loopBodyStartState);
    uint32_t loopBodyLastState = statementHandleDispatch(node->getChild(1), loopBodyStartState);

    addTransition(loopBodyLastState, nullptr, currentState);

    uint32_t afterLoopState = _cfg->getNumStates();
    _cfg->addState(afterLoopState);

    // when the loop condition is false
    ASTNode* conditionFalseNode = ASTNode::create(NodeType::Not, DataType::Bool, _program, {node->getChild(0)});

    addTransition(currentState, createStatement(conditionFalseNode), afterLoopState);
    return afterLoopState;
}

uint32_t CFGBuilder::handleParallelStatement(ASTNode *node, uint32_t currentState) {

    NFA* shuffledCFG = nullptr;
    Alphabet parallelProgramAlphabet;

    for (int i = 0; i < node->getNumChildren(); i ++) {
        ASTNode* oneParallelProgram = node->getChild(i);

        NFA* oneParallelProgramCFG = new NFA();
        CFGBuilder builder(oneParallelProgramCFG, oneParallelProgram, parallelProgramAlphabet, _program);
        uint32_t lastState = builder.build();
        oneParallelProgramCFG->setAcceptState(lastState);

        if (shuffledCFG == nullptr) {
            shuffledCFG = oneParallelProgramCFG;
        }
        else {
            NFA* lastShuffledCFG = shuffledCFG;
            shuffledCFG = shuffledCFG->shuffle(oneParallelProgramCFG, parallelProgramAlphabet);
            delete lastShuffledCFG;
            delete oneParallelProgramCFG;
        }
    }

    assert(shuffledCFG->getNumAcceptingStates() == 1 && "Error\n");

    uint32_t shuffledCFGLastState = *shuffledCFG->getAcceptStates().begin();
    shuffledCFG->unsetAcceptState(shuffledCFGLastState);

    // need to concatenate the shuffled CFG with the CFG before the parallel statement
    uint32_t stateOffset = _cfg->getNumStates();

    _cfg->concatenate(currentState, shuffledCFG);

    // union with the program alphabet
    _program->getAlphabet().insert(parallelProgramAlphabet.begin(), parallelProgramAlphabet.end());

    return shuffledCFGLastState + stateOffset;
}