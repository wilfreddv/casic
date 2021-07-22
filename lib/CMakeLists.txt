set(TARGET libcasic.o)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/libcasic.s
)

set(ASSEMBLE_OPTIONS -felf64)

add_custom_target(
    ${TARGET}
    COMMAND nasm ${SOURCES} ${ASSEMBLE_OPTIONS} -o ${CMAKE_BINARY_DIR}/${TARGET}
    COMMENT "Assembling libcasic"
)