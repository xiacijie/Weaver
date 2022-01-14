add_library(program
        ${CMAKE_CURRENT_LIST_DIR}/Program.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Statement.cpp
        )

target_link_libraries(program antlr4_static ast cfg frontend util automata)