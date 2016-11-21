set(app	n03_SponzaOpenGL)

set(deps Framework imgui)

foreach(dep ${Internal_deps})
  if(NOT TARGET ${dep})
    message(${LIB_NAME} " Missing target: " ${dep})
  endif()
endforeach(dep)

project(${app})
file(GLOB shaders ${FitGL_SOURCE_DIR}/Resources/Shaders/Nei/${app}/*)
file(GLOB src ${FitGL_SOURCE_DIR}/Apps/Nei/${app}/*)
add_executable(${app} ${src} ${shaders})
source_group("Shaders" FILES ${shaders})
target_link_libraries(${app} ${deps})
set_target_properties(${app} PROPERTIES FOLDER "Nei")
