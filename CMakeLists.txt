cmake_minimum_required(VERSION 3.14)

project(1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6Core REQUIRED)
find_package(Qt6Widgets REQUIRED)

add_executable(1
  main.cpp
  mainwindow.h mainwindow.cpp mainwindow.ui
  authwindow.h authwindow.cpp authwindow.ui
  windowsmanager.h windowsmanager.cpp
)
target_link_libraries(1 Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(1 Qt${QT_VERSION_MAJOR}::Widgets)

include(GNUInstallDirs)
install(TARGETS 1
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


