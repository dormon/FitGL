project(PGP2016)

set(deps FGL)

foreach(dep ${deps})
  if(NOT TARGET ${dep})
    message(${LIB_NAME} " Missing target: " ${dep})
	return()
  endif()
endforeach(dep)

file(GLOB shaders ${FitGL_SOURCE_DIR}/Resources/Shaders/PGP2016/*)
file(GLOB src ${FitGL_SOURCE_DIR}/Apps/PGP2016/*)
include_directories(${FitGL_SOURCE_DIR}/Apps/PGP2016/)
add_executable(PGP2016 ${src} ${shaders})
source_group("Shaders" FILES ${shaders})
target_link_libraries(PGP2016 ${deps})
set_target_properties(PGP2016 PROPERTIES FOLDER "PGP2016")