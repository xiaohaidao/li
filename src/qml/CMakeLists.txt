
project(qml)


include(CmakeLists.txt.in)
include(${CMAKEDIR}/common.cmake)

Findqt5(core quick)

add_executable(${BUILD} ${HEAD} ${SOURCE})

target_link_libraries(${BUILD} Qt5::Core Qt5::Quick)
