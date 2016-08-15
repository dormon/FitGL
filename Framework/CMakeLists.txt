set(LIB_NAME Framework)

set(HEADER_PATH ${FitGL_SOURCE_DIR}/${LIB_NAME})

set(FRAMEWORK_INCLUDES
	BaseApp.h
	SDLWindow/SDLEventProc.h
	SDLWindow/SDLWindow.h
		
  Shader.h
  #Camera.h
  LoadTextFile.h
  ProgramObject.h
  #Shader.h
  #Timer.h
  #ValuePrinter.h
  )

set(FRAMEWORK_SOURCES
	BaseApp.cpp
	SDLWindow/SDLEventProc.cpp
	SDLWindow/SDLWindow.cpp
  LoadTextFile.cpp  
  )

add_library(${LIB_NAME}
  ${FRAMEWORK_INCLUDES}
  ${FRAMEWORK_SOURCES}
  )


set(Internal_deps SDL2 GLEW glm)
set(Internal_inc ${FitGL_SOURCE_DIR}/Framework)
set(includes_to_export ${FitGL_SOURCE_DIR}/include)

target_link_libraries(${LIB_NAME} ${Internal_deps} ${External_libs} )
set_target_properties(${LIB_NAME} PROPERTIES
  INCLUDE_DIRECTORIES "${Internal_inc}"
  INTERFACE_INCLUDE_DIRECTORIES "${includes_to_export}"
  )


install(FILES
  ${FRAMEWORK_INCLUDES}
  DESTINATION include/${LIB_NAME}
  )

install(TARGETS ${LIB_NAME}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  )

