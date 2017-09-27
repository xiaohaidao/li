#project(http)

add_definitions(-DASIO_STANDALONE)

set(http_h
    header.h
    requests.h
    response.h
    Url.h)

set(http_src
    main.cpp
    requests.cpp
    Url.cpp)

add_executable(http ${http_src} ${http_h})
