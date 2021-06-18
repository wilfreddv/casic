cmake_minimum_required(VERSION 3.18)
project(casic C)

add_compile_options(-c -Wall -Werror -g -DDEBUG)

set(SOURCES
    src/AST.c
    src/casic.c
    src/lexer.c
    src/token.c
    src/parser.c
)

add_executable(casic ${SOURCES})

target_include_directories(casic PRIVATE include)


add_custom_target(test
    DEPENDS casic
    COMMAND ./casic ${CMAKE_SOURCE_DIR}/test/test.casic
    USES_TERMINAL
)
