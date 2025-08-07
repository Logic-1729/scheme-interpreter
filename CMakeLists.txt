cmake_minimum_required(VERSION 3.5)

project (scheme)

if(DEFINED ENV{ONLINE_JUDGE})
    add_definitions(-DONLINE_JUDGE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# 移除自定义的输出路径设置，使用默认的构建目录

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/syntax.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RE.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/expr.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/value.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/evaluation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Def.cpp
)

add_executable(code ${SOURCES})

# 设置 C++ 标准
set_target_properties(code PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)

target_compile_options(code
  PRIVATE
    -g
)
