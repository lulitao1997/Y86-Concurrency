cmake_minimum_required(VERSION 3.6)
project(Y86_Conc)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        controller.cpp
        csapp.c
        csapp.h
        map_io.cpp
        map_io.h
        sample_client.cpp
        consts.h)
set(CSAPP csapp.c csapp.h)
set(CONTROLLER controller.cpp ${CSAPP} webserver.cpp)
add_executable(CONTROLLER ${CONTROLLER} PIPE/Pipe.cpp PIPE/Transfer.cpp)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
target_link_libraries(CONTROLLER pthread m)
add_executable(TEST test.cpp)
add_subdirectory(PIPE)
