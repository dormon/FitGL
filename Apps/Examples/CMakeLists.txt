set(apps
	000_Callbacks
	001_Minimal
	002_MultipleViews
	003_MultipleWindows                
	004_Textures
	005_imgui
)

foreach(app ${apps})
	project(${app})
	file(GLOB shaders ${FitGL_SOURCE_DIR}/Resources/Shaders/Examples/${app}/*)
	file(GLOB src ${FitGL_SOURCE_DIR}/Apps/Examples/${app}/*)
	add_executable(${app} ${src} ${shaders})
	source_group("Shaders" FILES ${shaders})
	target_link_libraries(${app} Framework imgui)
endforeach()
