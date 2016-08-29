set(apps
	001_HelloTriangle
	002_Primitives
	0xx_Blending
	0xx_CubeMapping
	0xx_EnvieromentMapping
	0xx_FsRaytracing
	0xx_Interpolation
	0xx_Mipmap
	0xx_Noise
	0xx_NormalMapping
	0xx_ParalaxMapping
	0xx_Triplanar
	0xx_UV
	0xx_Wrapping
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
