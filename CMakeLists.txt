cmake_minimum_required(VERSION 3.4)

# The project name
project (LiH)

# The project version number.
set(VERSION_MAJOR   0   CACHE STRING "Project major version number.")
set(VERSION_MINOR   0   CACHE STRING "Project minor version number.")
set(VERSION_PATCH   1   CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH) # don't show in GUI,unless the advanced was selected

# set lib and exe target path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/libs)

# Generate compile_commands.json to ycm
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set variable of common cmake dir
set(CMAKEDIR ${PROJECT_SOURCE_DIR}/cmake)

# set include and link search path
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/3rdparty)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/libs)

# Set the Readme.md file into html
option(USER_DOXY_PAGE "Add the Readme.md to the main page in html what doxyen generated" false)
if (USER_DOXY_PAGE)
    set(doxy_main_page Readme.md)
endif()

# Add sub directory
add_subdirectory(src/*)
add_subdirectory(docs)
#add_subdirectory(include)

if(BUILD_TEST)
    enable_testing()
    add_subdirectory(tests)
    INCLUDE_DIRECTORIES(tests)
endif()
