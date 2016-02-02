CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/util/cmake/" ${CMAKE_MODULE_PATH})


# include Petsc
find_package(PETSc)
include_directories(${PETSC_INCLUDES})
include_directories(${CMAKE_SOURCE_DIR}/include)

ADD_EXECUTABLE(mytest mytest.cpp)
TARGET_LINK_LIBRARIES(mytest ${PETSC_LIBRARIES})
