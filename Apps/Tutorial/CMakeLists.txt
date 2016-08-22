set(apps
	001_HelloTriangle
	002_Primitives
)

foreach(app ${apps})
	project(${app})
	file(GLOB shaders ${FitGL_SOURCE_DIR}/Resources/Shaders/Tutorial/${app}/*)
	file(GLOB src ${FitGL_SOURCE_DIR}/Apps/Tutorial/${app}/*)
	add_executable(${app} ${src} ${shaders})
	source_group("Shaders" FILES ${shaders})
	target_link_libraries(${app} Framework imgui)
	set_target_properties(${app} PROPERTIES FOLDER "Tutorial")
endforeach()
