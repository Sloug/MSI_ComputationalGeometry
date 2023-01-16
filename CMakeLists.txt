#This file does the basic configuration of the whole Geometric Modeling 

#This Cmake version is minimum required to run Qt6
cmake_minimum_required(VERSION 3.16.0)

project(ComputationalGeometry)

set(PROJECT_MAIN_NAME "CM")
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 14)

if(CMAKE_CONFIGURATION_TYPE)
    set(CMAKE_CNFIGURATION_TYPES "DEBUG;RELEASE" CACHE STRING "Available Build Types")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()
ADD_CUSTOM_TARGET(debug
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
  COMMENT "Creating the executable in the debug mode.")

ADD_CUSTOM_TARGET(release
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
  COMMENT "Creating the executable in the release mode.")
  
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

find_package(Qt6 6.4.2 EXACT REQUIRED COMPONENTS Core Widgets OpenGL OpenGLWidgets)
get_target_property(QtCoreLocation Qt6::Core LOCATION)
get_filename_component(QT_LIB_DIR ${QtCoreLocation} PATH)
set(QT_LIB_DIR ${QT_LIB_DIR})
set(QT_BIN_DIR ${QT_LIB_DIR})
set(QT_INCLUDE_DIR ${QT_LIB_DIR}/../include)
set(QT_SRC_DIR ${QT_LIB_DIR}/../../Src)
#
message(STATUS "Qt library directory: " ${QT_LIB_DIR})
message(STATUS "Qt binary directory: " ${QT_BIN_DIR})
message(STATUS "Qt include directory: " ${QT_INCLUDE_DIR})
message(STATUS "Qt src directory: " ${QT_SRC_DIR})

add_subdirectory(src/Aufgabe1)
add_subdirectory(src/Aufgabe2)
add_subdirectory(src/Aufgabe3)
add_subdirectory(src/Aufgabe4)
