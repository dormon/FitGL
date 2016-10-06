# tests based on SimpleApp
set(apps
	001_TextureCompression
)

foreach(app ${apps})
	set(tar Test_${app})
	file(GLOB shaders ${FitGL_SOURCE_DIR}/Resources/Shaders/Tests/${app}/*)
	file(GLOB src ${FitGL_SOURCE_DIR}/Apps/Tests/${app}/*.cpp)
	add_executable(${tar} ${src} ${FitGL_SOURCE_DIR}/Framework/SimpleApp.cpp ${shaders})
	source_group("Shaders" FILES ${shaders})
	target_link_libraries(${tar} GLEW glm SDL2 OpenGL)	
	set_target_properties(${tar} PROPERTIES FOLDER "Tests")		
endforeach()