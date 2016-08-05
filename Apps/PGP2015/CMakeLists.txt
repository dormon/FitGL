set(list DU01 DU02 DU03 DU04 DU05)
if(GLEW_FOUND AND glm_FOUND AND SDL2_FOUND)	
	foreach(app ${list})
		file(GLOB shaders ${FitGL_SOURCE_DIR}/Resources/Shaders/PGP2015/${app}*)
		add_executable(PGP2015_${app} ${app}.cpp ${FitGL_SOURCE_DIR}/Framework/SimpleApp.cpp ${shaders})
		source_group("Shaders" FILES ${shaders})
		target_link_libraries(PGP2015_${app} GLEW glm SDL2)
	endforeach()
else()
	message("Can't build PGP2015 - missing libs")
endif()

#TODO install