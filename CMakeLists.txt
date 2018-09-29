cmake_minimum_required(VERSION 3.8)

# The project name
project (LiH VERSION 0.0.1)

# set lib and exe target path
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/libs")

# Generate compile_commands.json to ycm
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set variable of common cmake dir
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# set include and link search path
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src" "${PROJECT_SOURCE_DIR}/3rdparty")
LINK_DIRECTORIES("${PROJECT_SOURCE_DIR}/libs")

INCLUDE(common)

# Add a customer command to generate API documentation with Doxygen
GenerateDoc()

# Add sub directory
ADD_SUBDIRLIST(CURDIR "${PROJECT_SOURCE_DIR}/src")

if(BUILD_TEST)
    enable_testing()
    add_subdirectory(tests)
    INCLUDE_DIRECTORIES(tests)
endif()
