cmake_minimum_required(VERSION 3.0.0)

PROJECT(FitGL)

IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")# -W -Wall")# -pedantic")
ENDIF()

set(CMAKE_MODULE_PATH "${FitGL_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(GLEW          REQUIRED)
find_package(OpenGL        REQUIRED)
find_package(SDL2          REQUIRED)
find_package(glm           REQUIRED)
find_package(FreeImage     QUIET)
find_package(FreeImagePlus QUIET)
find_package(assimp QUIET)

include_directories(${FitGL_SOURCE_DIR}/Framework)

add_subdirectory(3rdParty/imgui)
add_subdirectory(Framework)

# PGP2015 apps       
set(FITGL_BUILD_PGP2015 ON CACHE BOOL "Build PGP2015")
if(FITGL_BUILD_PGP2015)
	add_subdirectory(Apps/PGP2015)
endif()   

# tests
set(FITGL_BUILD_TESTS ON CACHE BOOL "Build all test")
if(FITGL_BUILD_TESTS)
	add_subdirectory(Apps/Tests)
endif()

# tutorial
set(FITGL_BUILD_TUTORIALS ON CACHE BOOL "Build all tutorials")
if(FITGL_BUILD_TUTORIALS)
	add_subdirectory(Apps/Tutorial)
endif()

# examples
set(FITGL_BUILD_EXAMPLES ON CACHE BOOL "Build all examples")
if(FITGL_BUILD_EXAMPLES)
	add_subdirectory(Apps/Examples)
endif()
	
# copy resources - whole dir	
add_custom_command(
    OUTPUT res
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources/ ${CMAKE_CURRENT_BINARY_DIR}/Resources/
    )
add_custom_target(CopyResources ALL DEPENDS res)
