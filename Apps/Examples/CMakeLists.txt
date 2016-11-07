set(apps
	e00_Callbacks
	e01_Minimal
	e02_MultipleViews
	e03_MultipleWindows                
	e04_Textures
	e05_Imgui
	e06_ModelLoader
	e07_Timer
	e08_OrbitManipulator
  e09_3DPiskvorky
	e10_Extensions
  e11_Tesselation
  e12_EndlessTerrain
)

foreach(app ${apps})
	project(${app})
	file(GLOB shaders ${FitGL_SOURCE_DIR}/Resources/Shaders/Examples/${app}/*)
	file(GLOB src ${FitGL_SOURCE_DIR}/Apps/Examples/${app}/*)
	add_executable(${app} ${src} ${shaders})
	source_group("Shaders" FILES ${shaders})
	target_link_libraries(${app} Framework imgui)
	set_target_properties(${app} PROPERTIES FOLDER "Examples")
endforeach()
