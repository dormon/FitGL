set(deps FGL)

foreach(dep ${deps})
  if(NOT TARGET ${dep})
    message(${LIB_NAME} " Missing target: " ${dep})
	return()
  endif()
endforeach(dep)


set(list DU01 DU02 DU03 DU04 DU05)

foreach(app ${list})
	set(tar PGP2015_${app})
	project(${tar})
	file(GLOB shaders ${FitGL_SOURCE_DIR}/Resources/Shaders/PGP2015/${app}*)
	add_executable(${tar} ${app}.cpp ${shaders})
	source_group("Shaders" FILES ${shaders})
	target_link_libraries(${tar} ${deps})
	set_target_properties(${tar} PROPERTIES FOLDER "PGP2015")
endforeach()