
macro(AddExecute)
    set(options)
    set(oneValueArgs TARGET)
    set(multiValueArgs SOURCE QT_MODULE)
    cmake_parse_arguments(AddExecute "${options}" "${oneValueArgs}"
        "${multiValueArgs}" ${ARGN})

    string(LENGTH "${AddExecute_QT_MODULE}" HAVE_QT_MODULE)
    if("${HAVE_QT_MODULE}")
        find_package(Qt5 COMPONENTS ${AddExecute_QT_MODULE} REQUIRED)
        if(NOT Qt5_FOUND)
            message(FATAL_ERROR "QT not find. you can set CMAKE_PREFIX_PATH/Qt5_DIR!")
        endif()

        set(CMAKE_AUTOMOC ON)
        set(CMAKE_AUTORCC ON)
        set(CMAKE_AUTOUIC ON)
        set(CMAKE_INCLUDE_CURRENT_DIR ON)
    endif()
    add_executable("${AddExecute_TARGET}" "${AddExecute_SOURCE}")

    set_target_properties("${AddExecute_TARGET}" PROPERTIES DEBUG_POSTFIX d)

    if("${HAVE_QT_MODULE}")
        qt5_use_modules("${AddExecute_TARGET}" "${AddExecute_QT_MODULE}")
    endif()
endmacro()

function(GenerateDoc)
    find_package(Doxygen QUIET)
    option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

    if(BUILD_DOCUMENTATION)
        if(NOT DOXYGEN_FOUND)
            message(FATAL_ERROR "Doxygen is needed to build the documentation.")
        endif()

        # Set the Readme.md file into html
        option(USER_DOXY_PAGE "Add the Readme.md to the main page in html what doxyen generated" false)
        if (USER_DOXY_PAGE)
            set(doxy_main_page Readme.md)
        endif()

        set(doxyfile_in "${CMAKE_MODULE_PATH}/template/Doxyfile.in")
        set(doxyfile "${CMAKE_BINARY_DIR}/Doxyfile")

        configure_file("${doxyfile_in}" "${doxyfile}" @ONLY)

        add_custom_target(doc
            COMMAND "${DOXYGEN_EXECUTABLE}" "${doxyfile}"
            WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/docs"
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)

        #install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc)
    endif()
endfunction()

function(ADD_SUBDIRLIST)
    set(options)
    set(oneValueArgs CURDIR RESULT)
    set(multiValueArgs)
    cmake_parse_arguments(ADD_SUBDIRLIST "${options}" "${oneValueArgs}"
        "${multiValueArgs}" ${ARGN})

    FILE(GLOB children RELATIVE "${ADD_SUBDIRLIST_CURDIR}" "${ADD_SUBDIRLIST_CURDIR}/*")
    FOREACH(child ${children})
        IF(IS_DIRECTORY "${ADD_SUBDIRLIST_CURDIR}/${child}")
            ADD_SUBDIRECTORY("${ADD_SUBDIRLIST_CURDIR}/${child}")
        ENDIF()
    ENDFOREACH()
endfunction()
