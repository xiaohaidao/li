project(http)

add_definitions(-DASIO_STANDALONE)
if(CYGWIN)
    add_definitions(-D__USE_W32_SOCKETS)
endif()

set(http_h
    header.h
    requests.h
    Url.h)

set(http_src
    main.cpp
    header.cpp
    requests.cpp
    Url.cpp)

include(${CMAKEDIR}/common.cmake)

use_cxx11()

add_executable(${BUILD} ${http_src} ${http_h})
