cmake_minimum_required(VERSION 3.7.1)
project(qtosg-hello)

#set(Qt5_DIR "*PATH TO YOUR QT QT5CONFIG FILE HERE*" CACHE PATH "Initial cahe" FORCE)
set(Qt5_DIR "/home/parminder/Qt5.12.7/5.12.7/gcc_64/lib/cmake/Qt5" CACHE PATH "Initial cahe" FORCE)
set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui OpenGL)
find_package(OpenSceneGraph REQUIRED COMPONENTS osgDB osgGA osgUtil osgViewer)

include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

set(SOURCES
    main.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    ${OPENSCENEGRAPH_LIBRARIES}
    Qt5::Core
    Qt5::Gui
    Qt5::OpenGL
    )

# Allow the project to run with OSG or predefine Cylinder geometry shape
add_definitions(-DUSE_OSG=1)
