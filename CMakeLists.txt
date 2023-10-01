# Specify the minimum Cmake version required
cmake_minimum_required(VERSION 3.0)

# Set the project name and version
project(kshell VERSION 0.1)

# Specify the C compiler
set(CMAKE_C_COMPILER gcc)

# Define the source files
file(GLOB SOURCES "src/*.c")

# Include directories
include_directories(include)

# Create the executable
add_executable(kshell ${SOURCES})

# Linked Libraries
target_link_libraries(kshell PRIVATE readline)
