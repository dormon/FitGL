set(app	n03_SponzaOpenGL)

set(deps FGL)

foreach(dep ${deps})
  if(NOT TARGET ${dep})
    message(${LIB_NAME} " Missing target: " ${dep})
	return()
  endif()
endforeach(dep)

project(${app})
file(GLOB shaders ${FitGL_SOURCE_DIR}/Resources/Shaders/Nei/${app}/*)
file(GLOB src ${FitGL_SOURCE_DIR}/Apps/Nei/${app}/*)
add_executable(${app} ${src} ${shaders})
source_group("Shaders" FILES ${shaders})
target_link_libraries(${app} ${deps})
set_target_properties(${app} PROPERTIES FOLDER "Nei")
