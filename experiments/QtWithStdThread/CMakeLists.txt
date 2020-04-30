project(QtWithStdThread)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

find_package(Threads)

add_executable(QtWithStdThread
    "QtWithStdThread.cpp"
    "mainwindow.cpp"
    "mainwindow.h"
    "mainwindow.ui"
)

target_link_libraries(QtWithStdThread
    PRIVATE Qt5::Widgets
    ${CMAKE_THREAD_LIBS_INIT}
)
