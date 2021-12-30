add_library(theoremprover
        ${CMAKE_CURRENT_LIST_DIR}/TheoremProverBase.cpp
        ${CMAKE_CURRENT_LIST_DIR}/MathSAT.cpp
        ${CMAKE_CURRENT_LIST_DIR}/SMTInterpol.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Yices.cpp
        )