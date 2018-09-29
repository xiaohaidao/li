
project(qml VERSION 0.0.1)

set(PUBLIC_HEADER_)

set(SOURCE_
    main.cpp
    qml.qrc
    )

set(QT_MODULE_ Quick Qml)

AddExecute(TARGET "${PROJECT_NAME}"
    SOURCE "${SOURCE_}"
    QT_MODULE "${QT_MODULE_}")

