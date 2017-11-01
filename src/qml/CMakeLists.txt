
project(qml)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORRC ON)
find_package(Qt5 COMPONENTS Core Quick REQUIRED)

if(NOT Qt5_FOUND)
    message(FATAL_ERROR "QT not find. you can set CMAKE_PREFIX_PATH!")
endif()

include(CmakeLists.txt.in)
include(${CMAKEDIR}/common.cmake)

add_executable(${BUILD} ${HEAD} ${SOURCE})

target_link_libraries(${BUILD} Qt5::Core Qt5::Quick)
