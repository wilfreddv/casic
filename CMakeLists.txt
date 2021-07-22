cmake_minimum_required(VERSION 3.18)
project(casic C ASM_NASM)
enable_language(ASM_NASM)


add_subdirectory(lib)


set(SOURCES
    src/AST.c
    src/casic.c
    src/lexer.c
    src/token.c
    src/parser.c
    src/intermediate.c
)

add_executable(casic ${SOURCES})
target_compile_options(casic PRIVATE -c -Wall -Werror -g -DDEBUG)
target_include_directories(casic PRIVATE include)


add_custom_target(test
    DEPENDS casic
    COMMAND ./casic ${CMAKE_SOURCE_DIR}/test/test.casic
    USES_TERMINAL
)

add_custom_target(test-no-compile
    DEPENDS casic
    COMMAND ./casic ${CMAKE_SOURCE_DIR}/test/test.casic -S
    USES_TERMINAL
)

add_custom_target(linecount
    COMMAND grep -r . ${CMAKE_SOURCE_DIR} --exclude-dir={${PROJECT_BINARY_DIR}} | wc -l
)