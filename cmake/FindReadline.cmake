# Custom FindReadline.cmake module 
# Not needed anymore but keeping for reference

# Define a variable to hold the search paths
set(READLINE_SEARCH_PATHS
    /usr/lib/x86_64-linux-gnu
)

# Define a variable to hold the possible library names
set(READLINE_LIBRARY_NAMES
    readline
)

# Search for the readline library
find_library(READLINE_LIBRARY
    NAMES ${READLINE_LIBRARY_NAMES}
    PATHS ${READLINE_SEARCH_PATHS}
)

# Check if the library was found
if(READLINE_LIBRARY)
    message(STATUS "Found Readline library: ${READLINE_LIBRARY}")
else()
    message(FATAL_ERROR "Readline library not found")
endif()

# Define a variable to hold the include directory
set(READLINE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Provide the include directory and library as variables
set(READLINE_FOUND TRUE)

# Export the variables
set(READLINE_INCLUDE_DIRS ${READLINE_INCLUDE_DIR})
set(READLINE_LIBRARIES ${READLINE_LIBRARY})

