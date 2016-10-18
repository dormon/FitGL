set(app	001_Test)

project(${app})
file(GLOB shaders ${FitGL_SOURCE_DIR}/Resources/Shaders/Nei/${app}/*)
file(GLOB src ${FitGL_SOURCE_DIR}/Apps/Nei/${app}/*)
add_executable(${app} ${src} ${shaders})
source_group("Shaders" FILES ${shaders})
target_link_libraries(${app} NeiVu)
set_target_properties(${app} PROPERTIES FOLDER "Nei")
