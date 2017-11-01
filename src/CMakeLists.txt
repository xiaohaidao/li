
set(subdir http qml)

foreach(dirs ${subdir})
    add_subdirectory(${dirs})
endforeach()
