cmake_minimum_required (VERSION 2.8.0)
PROJECT(capture CXX)
ENABLE_LANGUAGE(C)

#################################################################
# Include required CMake modules
#################################################################
INCLUDE(FindPkgConfig)
INCLUDE(CheckIncludeFiles)
INCLUDE(CheckIncludeFileCXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/")
#include(GetGitRevisionDescription)

#################################################################
# Get the version of this build
#################################################################
#git_describe(YURI_GIT_VERSION)

#################################################################
# Add strict warning if building under linux
#################################################################
IF (UNIX)
	add_definitions("-Wall -Wextra -std=c++11") 
ENDIF ()

include_directories( ${CMAKE_SOURCE_DIR}/src )
SET (EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(MotionClassification)

