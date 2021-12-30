
add_library(verifier
        ${CMAKE_CURRENT_LIST_DIR}/InterpolantAutomataBuilder.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ParallelProgramVerifier.cpp
        ${CMAKE_CURRENT_LIST_DIR}/SequentialProgramVerifier.cpp
)