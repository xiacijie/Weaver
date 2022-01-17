add_library(theoremprover
        ${CMAKE_CURRENT_LIST_DIR}/MathSAT.cpp
        ${CMAKE_CURRENT_LIST_DIR}/SMTInterpol.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Yices.cpp
        ${CMAKE_CURRENT_LIST_DIR}/SSANumberingTable.cpp
        ${CMAKE_CURRENT_LIST_DIR}/SMTSolverBase.cpp
        )