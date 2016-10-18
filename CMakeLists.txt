cmake_minimum_required(VERSION 3.0.0)

PROJECT(FitGL)

#prebuild lib package                                 
set(FITGL_USE_LIB_PACKAGE OFF CACHE BOOL "Use prebuild lib package.")
if(FITGL_USE_LIB_PACKAGE)
	set(ASSIMP_DLL_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/assimp-vc140-mt.dll)
	set(ASSIMP_DLL_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/assimp-vc140-mt.dll)
	set(ASSIMP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/Assimp/include)
	set(ASSIMP_LIBRARY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/libs/Assimp/lib/assimp-vc140-mt.lib)
	set(ASSIMP_LIBRARY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/libs/Assimp/lib/assimp-vc140-mt.lib)
	
	
	set(FREEIMAGE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/FreeImage CACHE STRING "" FORCE)
	set(FREEIMAGE_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libs/FreeImage/FreeImage.lib CACHE STRING "" FORCE)
	set(FREEIMAGEPLUS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/FreeImage CACHE STRING "" FORCE)
	set(FREEIMAGEPLUS_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libs/FreeImage/FreeImagePlus.lib CACHE STRING "" FORCE)
	set(GLEW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/glew/include CACHE STRING "" FORCE)
	set(GLEW_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libs/glew/lib/glew32.lib CACHE STRING "" FORCE)    
	set(GLM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/glm/include CACHE STRING "" FORCE)
	set(SDL2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/include CACHE STRING "" FORCE)
	set(SDL2_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/lib/SDL2.lib CACHE STRING "" FORCE)
	set(SDL2MAIN_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/lib/SDL2main.lib CACHE STRING "" FORCE)
	set(SDL2_LIBRARY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/lib/SDL2.lib CACHE STRING "" FORCE)

	# copy dlls	
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/bin DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()  

# send all exes to bin/ dir
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
# Second, for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )


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
find_package(vulkan QUIET)

include_directories(${FitGL_SOURCE_DIR}/Framework)

add_definitions(-DRESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Resources/")

add_subdirectory(3rdParty/imgui)
add_subdirectory(Framework)

# NeiVU
set(FITGL_BUILD_NEIVU OFF CACHE BOOL "Build vulkan lib")
if(FITGL_BUILD_NEIVU)
	add_subdirectory(NeiVu)
endif()
set(FITGL_BUILD_NEI_APPS OFF CACHE BOOL "Build nei apps")
if(FITGL_BUILD_NEI_APPS)
	add_subdirectory(Apps/Nei)
endif()

# PGP2015 apps       
set(FITGL_BUILD_PGP2015 ON CACHE BOOL "Build PGP2015")
if(FITGL_BUILD_PGP2015)
	add_subdirectory(Apps/PGP2015)
endif()  

# PGP2016 apps       
set(FITGL_BUILD_PGP2016 ON CACHE BOOL "Build PGP2016")
if(FITGL_BUILD_PGP2016)
	add_subdirectory(Apps/PGP2016)
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
add_custom_target(CopyResources DEPENDS res)
