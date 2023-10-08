# Specify the minimum Cmake version required
cmake_minimum_required(VERSION 3.0)

# Set the project name and version
project(kshell VERSION 0.1)

# Specify the C compiler
set(CMAKE_C_COMPILER clang)

# Generate a compile comands file for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define the source files
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

# Include directories
set(CMAKE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("${CMAKE_INCLUDE_DIR}")

# Create the executable
add_executable(kshell ${SOURCES})

# Generate Assembly code for each source file individually
foreach(source_file ${SOURCES})
  get_filename_component(asm_output "${source_file}" NAME_WE)
  add_custom_target(GenerateAssemblyCode_${asm_output}
    #clang -S -mllvm --x86-asm-syntax=intel -I ../include main.c
    COMMAND ${CMAKE_C_COMPILER} -S -mllvm --x86-asm-syntax=intel -I${CMAKE_INCLUDE_DIR} ${source_file}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generating assembly code for ${asm_output}..."
  )
  add_dependencies(kshell GenerateAssemblyCode_${asm_output})
endforeach()

# Linked Libraries
target_link_libraries(kshell readline)



