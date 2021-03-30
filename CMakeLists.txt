cmake_minimum_required(VERSION 3.8)

project(byol VERSION 0.1.0 
    LANGUAGES C CXX)


# mpc library
add_library(mpc ext_deps/mpc-0.9.0/mpc.c)

# main
add_executable(byol src/byol.cpp)

# LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# compiler options
target_compile_features(byol PRIVATE cxx_std_20)
target_compile_options(byol PRIVATE -Wall -Wextra )
target_link_libraries(byol PRIVATE mpc readline)
target_include_directories(byol PRIVATE ext_deps/mpc-0.9.0/)

# copy compile commands
# copy compile commands to project root dir
add_custom_target(
    copy-compile-commands ALL
    ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_LIST_DIR}
    )
