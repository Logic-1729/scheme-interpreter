cmake_minimum_required(VERSION 3.5)

project (scheme)

if(DEFINED ENV{ONLINE_JUDGE})
    add_definitions(-DONLINE_JUDGE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/syntax.cpp
    ${PROJECT_SOURCE_DIR}/src/RE.cpp
    ${PROJECT_SOURCE_DIR}/src/parser.cpp
    ${PROJECT_SOURCE_DIR}/src/expr.cpp
    ${PROJECT_SOURCE_DIR}/src/value.cpp
    ${PROJECT_SOURCE_DIR}/src/evaluation.cpp
    ${PROJECT_SOURCE_DIR}/src/Def.cpp
)

add_executable(code ${SOURCES})

target_compile_options(code
  PRIVATE
    -g
)
