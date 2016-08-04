cmake_minimum_required(VERSION 3.0.0)

PROJECT(FitGL)

set(FRAMEWORK_INCLUDES
  Framework/Shader.h
  )

set(FRAMEWORK_SOURCES
  )

include_directories(include)

IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -W -Wall -pedantic")
ENDIF()

set(CMAKE_MODULE_PATH "${opengl_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

find_package(GLEW          REQUIRED)
find_package(OpenGL        REQUIRED)
find_package(SDL2          REQUIRED)
find_package(glm           QUIET)
find_package(FreeImage     QUIET)
find_package(FreeImagePlus QUIET)
#find_package(OpenCV        REQUIRED)

#set(APP_NAME opengl)

#add_executable(${APP_NAME} ${SOURCES} ${INCLUDES} src/triangle.cpp)
#target_link_libraries(${APP_NAME} OpenGL GLEW glm SDL2)

function(add_binary)
  foreach(TARG ${S_TARGETS})
    add_executable(${TARG} ${SOURCES} ${INCLUDES} src/${TARG}.cpp)
    target_link_libraries(${TARG} OpenGL GLEW glm SDL2 FreeImage FreeImagePlus)# opencv_highgui opencv_core opencv_imgproc)
  endforeach(TARG)
endfunction(add_binary)

set(S_TARGETS
  #triangle
  #orbitManipulator
  #interpolation
  #sphere
  #noise
  #zfight
  #blending
  #uv
  #triplanar
  #cubeMapping
  #environmentMapping
  #wrapping
  #mipmap
  #normalMapping
  #paralaxMapping
  #fsRayTracing
  #rasterizationOrder
  #memoryBandwidth
  #prefixSum
  #prefixSum2
  #video
  #video_gray
  #video_sobel
  #video_gauss
  #video_random
  )
add_binary()

add_custom_target(copy-shaders ALL
  COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/shaders ${CMAKE_BINARY_DIR}/shaders
  DEPENDS ${MY_TARGET})

add_custom_target(copy-textures ALL
  COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/textures ${CMAKE_BINARY_DIR}/textures
  DEPENDS ${MY_TARGET})

#add_custom_target(copy-videos ALL
#  COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/videos ${CMAKE_BINARY_DIR}/videos
#  DEPENDS ${MY_TARGET})

