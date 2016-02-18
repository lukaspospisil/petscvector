# --------------------------------------------------
# PetscVector test files
# Lukas Pospisil, USI Lugano, 2016
#
# --------------------------------------------------


project(TEST_PETSCVECTOR)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# define some colors for funny cmake messages
if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(ColourBold  "${Esc}[1m")
  set(Red         "${Esc}[31m")
  set(Green       "${Esc}[32m")
  set(Yellow      "${Esc}[33m")
  set(Blue        "${Esc}[34m")
endif()

# CMAKE: include cmake functions
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/util/cmake/" ${CMAKE_MODULE_PATH})

# yes, this code will be surely compiled with petsc
message(STATUS "${Red}The code is compiled with PETSc${ColourReset}")
	
# PETSc: defined paths in ENV?
if(NOT DEFINED ENV{PETSC_DIR} OR NOT DEFINED ENV{PETSC_INCLUDES})
	message(STATUS "${Red}PETSC_DIR or PETSC_INCLUDES are not specified, trying to run find_package(PETSc)${ColourReset}")

	# magic function from Jed Brown
	find_package(PETSc)

endif()

# give info about paths
message(STATUS "${Yellow}PETSC_DIR = $ENV{PETSC_DIR} ${ColourReset}")
message(STATUS "${Yellow}PETSC_ARCH = $ENV{PETSC_DIR} ${ColourReset}")
message(STATUS "${Yellow}PETSC_INCLUDES = ${PETSC_INCLUDES} ${ColourReset}")

# PETSc: include petsc stuff
include_directories(${PETSC_INCLUDES})

# PETSCVECTOR: define variables for include directories
set(PETSCVECTOR_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
message(STATUS "${Yellow}PETSCVECTOR_INCLUDE = ${PETSCVECTOR_INCLUDE} ${ColourReset}")
include_directories(${PETSCVECTOR_INCLUDE})


# add test files
ADD_EXECUTABLE(assign assign.cpp)
TARGET_LINK_LIBRARIES(assign ${PETSC_LIBRARIES})



