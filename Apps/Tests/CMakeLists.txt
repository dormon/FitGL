# tests based on SimpleApp
set(apps
	001_TextureCompression
)

foreach(app ${apps})
	file(GLOB shaders ${FitGL_SOURCE_DIR}/Resources/Shaders/Tests/${app}/*)
	file(GLOB src ${FitGL_SOURCE_DIR}/Apps/Tests/${app}/*.cpp)
	add_executable(Test_${app} ${src} ${FitGL_SOURCE_DIR}/Framework/SimpleApp.cpp ${shaders})
	source_group("Shaders" FILES ${shaders})
	target_link_libraries(Test_${app} GLEW glm SDL2)			
endforeach()