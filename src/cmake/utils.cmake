cmake_minimum_required(VERSION 2.8)

set(TARGETS CACHE INTERNAL "Targets" FORCE)

macro(ADD_LIBRARY_CG target)
    add_library(${target} ${ARGN})
    set(TARGETS "${TARGETS};${target}" CACHE INTERNAL "Targets")
endmacro()

macro(ADD_EXECUTABLE_CG target)
    add_executable(${target} ${ARGN})
    set(TARGETS "${TARGETS};${target}" CACHE INTERNAL "Targets")
endmacro()

macro(GENERATE_COMPLETION)
    foreach(target ${TARGETS})
        get_target_property(dirs ${target} INCLUDE_DIRECTORIES)
        file(WRITE "${${target}_SOURCE_DIR}/.clang_complete" "")
        foreach(dir ${dirs})
            file(APPEND "${${target}_SOURCE_DIR}/.clang_complete" "-I ${dir}\n")
        endforeach()
    endforeach()
endmacro()
