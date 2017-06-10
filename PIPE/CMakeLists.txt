cmake_minimum_required(VERSION 3.6)
project(PIPE)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        pipe.h
        bin2ins.cpp
        main.cpp
        HardwareUnits.h
        Transfer.cpp
        Pipe.cpp
        Const.h
        bin2ins.h
        transfer.h
        LogicalUnits.h)
set(PIPE ${SOURCE_FILES})
add_executable(PIPE ${PIPE})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})