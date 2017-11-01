project(http)

add_definitions(-DASIO_STANDALONE)

set(http_h
    header.h
    requests.h
    Url.h)

set(http_src
    main.cpp
    header.cpp
    requests.cpp
    Url.cpp)

add_executable(${PROJECT_NAME} ${http_src} ${http_h})
