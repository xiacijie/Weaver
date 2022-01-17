
add_library(verifier
        ${CMAKE_CURRENT_LIST_DIR}/VerifierBase.cpp
        ${CMAKE_CURRENT_LIST_DIR}/LoopingTreeAutomataVerifier.cpp
        ${CMAKE_CURRENT_LIST_DIR}/FiniteAutomataVerifier.cpp
)