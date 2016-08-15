set(list
000_Callbacks
)

foreach(app ${list})
	project(${app})
	file(GLOB shaders ${FitGL_SOURCE_DIR}/Resources/Shaders/Examples/${app}*)
	file(GLOB src ${FitGL_SOURCE_DIR}/Apps/Examples/${app}/*)
	add_executable(${app} ${src})
	source_group("Shaders" FILES ${shaders})
	target_link_libraries(${app} Framework)
endforeach()
