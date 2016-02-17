project(TEST_PETSCVECTOR)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#define some colors for fun
if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(ColourBold  "${Esc}[1m")
  set(Red         "${Esc}[31m")
  set(Green       "${Esc}[32m")
  set(Yellow      "${Esc}[33m")
  set(Blue        "${Esc}[34m")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/util/cmake/" ${CMAKE_MODULE_PATH})

message(STATUS "${Red}The code is compiled with PETSc${ColourReset}")
	
# PETSc: include
if(NOT DEFINED ENV{PETSC_DIR} OR NOT DEFINED ENV{PETSC_INCLUDES})
	message(STATUS "${Red}PETSC_DIR or PETSC_INCLUDES are not specified, trying to run find_package(PETSc)${ColourReset}")
	find_package(PETSc)


endif()

message(STATUS "${Yellow}PETSC_DIR = $ENV{PETSC_DIR} ${ColourReset}")
message(STATUS "${Yellow}PETSC_ARCH = $ENV{PETSC_DIR} ${ColourReset}")
message(STATUS "${Yellow}PETSC_INCLUDES = ${PETSC_INCLUDES} ${ColourReset}")


include_directories(${PETSC_INCLUDES})


# PETSCVECTOR: define variables for include directories
set(PETSCVECTOR_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
message(STATUS "${Yellow}PETSCVECTOR_INCLUDE = ${PETSCVECTOR_INCLUDE} ${ColourReset}")
include_directories(${PETSCVECTOR_INCLUDE})


ADD_EXECUTABLE(mytest mytest.cpp)
TARGET_LINK_LIBRARIES(mytest ${PETSC_LIBRARIES})
