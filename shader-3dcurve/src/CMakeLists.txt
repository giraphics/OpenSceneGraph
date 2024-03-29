cmake_minimum_required(VERSION 2.8.11)
project(shader-3dcurve)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# Linux Nitro Qt path
#set(Qt5_DIR "/home/parminder/Qt5.12.7/5.12.7/gcc_64/lib/cmake/Qt5" CACHE PATH "Initial cahe" FORCE)

# Linux Thinkpad Qt path
set(Qt5_DIR "/home/parminder/Qt5.14.2/5.14.2/gcc_64/lib/cmake/Qt5" CACHE PATH "Initial cahe" FORCE)

find_package(OpenSceneGraph REQUIRED COMPONENTS osgDB osgGA osgUtil osgViewer)

include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

set(SOURCES
    osgShader.cpp
    Callbacks.h
    Data.h
    )

file(GLOB SHADER_SRC
    Shaders/*.geom
    Shaders/*.frag
    Shaders/*.vert
)

add_executable(${PROJECT_NAME} ${SHADER_SRC} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    ${OPENSCENEGRAPH_LIBRARIES}
    )

# copy shader files
foreach(shaderpath ${SHADER_SRC})
    get_filename_component(shader ${shaderpath} NAME)
    configure_file("${CMAKE_SOURCE_DIR}/Shaders/${shader}" "${CMAKE_BINARY_DIR}/Shaders/${shader}" COPYONLY)
endforeach()
