CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/util/cmake/" ${CMAKE_MODULE_PATH})

# PETSc: include
#if(NOT DEFINED ENV{PETSC_DIR})
	message(STATUS "PETSC_DIR is not specified, trying to run find_package(PETSc)")
	find_package(PETSc)
#endif()

message(STATUS "PETSC_DIR = $ENV{PETSC_DIR}")
message(STATUS "PETSC_ARCH = $ENV{PETSC_ARCH}")
message(STATUS "PETSC_INCLUDES = $ENV{PETSC_INCLUDES}" )


include_directories("/home/lukas/soft/petsc-3.5.4/include")
include_directories("/home/lukas/soft/petsc-3.5.4/arch-linux2-c-debug/include")
include_directories(${CMAKE_SOURCE_DIR}/include)

ADD_EXECUTABLE(mytest mytest.cpp)
TARGET_LINK_LIBRARIES(mytest ${PETSC_LIBRARIES})
