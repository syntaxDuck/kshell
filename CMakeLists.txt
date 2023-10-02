# Specify the minimum Cmake version required
cmake_minimum_required(VERSION 3.0)

# Set the project name and version
project(kshell VERSION 0.1)

# Specify the C compiler
set(CMAKE_C_COMPILER clang)

# Generate a compile comands file for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Append cmake directory to CMAKE_MODULE_PATH
#list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Specify external library and include directory
#find_package(Readline REQUIRED)

# Define the source files
file(GLOB SOURCES "src/*.c")

# Include directories
include_directories(include)

# Create the executable
add_executable(kshell ${SOURCES})

# Linked Libraries
target_link_libraries(kshell readline)
