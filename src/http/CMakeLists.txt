project(http VERSION 0.0.1)

add_definitions(-DASIO_STANDALONE)
if(CYGWIN)
    add_definitions(-D__USE_W32_SOCKETS)
endif()

set(SOURCE_
    main.cpp
    header.h
    header.cpp
    requests.h
    requests.cpp
    Url.h
    Url.cpp
    )

AddExecute(TARGET "${PROJECT_NAME}" SOURCE "${SOURCE_}")

