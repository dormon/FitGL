cmake_minimum_required(VERSION 3.0.0)

PROJECT(FitGL)

IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -W -Wall -pedantic")
ENDIF()

set(CMAKE_MODULE_PATH "${FitGL_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

find_package(GLEW          REQUIRED)
find_package(OpenGL        REQUIRED)
find_package(SDL2          REQUIRED)
find_package(glm           QUIET)
find_package(FreeImage     QUIET)
find_package(FreeImagePlus QUIET)

include_directories(${FitGL_SOURCE_DIR}/Framework)


add_subdirectory(Framework)

set(FITGL_BUILD_TUTORIALS ON CACHE BOOL "Build all tutorials")

set(FITGL_BUILD_EXAMPLES ON CACHE BOOL "Build all examples")


# PGP2015 apps       
set(FITGL_BUILD_PGP2015 ON CACHE BOOL "Build PGP2015")
if(FITGL_BUILD_PGP2015)
	add_subdirectory(Apps/PGP2015)
endif()


# tests
set(FITGL_BUILD_TESTS ON CACHE BOOL "Build all test")
if(FITGL_BUILD_TESTS)
	
	# tests based on SimpleApp
	set(apps
		001_TextureCompression
	)
	
	foreach(app ${apps})
		file(GLOB shaders Resources/Shaders/Tests/${app}/*)
		file(GLOB src Apps/Tests/${app}/*.cpp)
		add_executable(Test_${app} ${src} ${FitGL_SOURCE_DIR}/Framework/SimpleApp.cpp ${shaders})
		source_group("Shaders" FILES ${shaders})
		target_link_libraries(Test_${app} GLEW glm SDL2)			
	endforeach()
	
	

endif()

