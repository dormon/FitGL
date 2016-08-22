set(LIB_NAME Framework)

set(HEADER_PATH ${FitGL_SOURCE_DIR}/${LIB_NAME})

set(FRAMEWORK_INCLUDES
	BaseApp.h
	SDLWindow/SDLEventProc.h
	SDLWindow/SDLWindow.h
		
	Shader.h
	ProgramObject.h
	Loader.h
	Debugging.h   
	Gui.h
	Mesh.h
	Image.h
	Node.h
	Material.h
  #Camera.h
  #LoadTextFile.h  
  #Timer.h
  #ValuePrinter.h
  )

set(FRAMEWORK_SOURCES
	BaseApp.cpp
	SDLWindow/SDLEventProc.cpp
	SDLWindow/SDLWindow.cpp
	LoaderText.cpp
	LoaderAssimp.cpp
	LoaderTexture.cpp
	Gui.cpp
	Mesh.cpp
	Node.cpp
	Material.cpp
  )

add_library(${LIB_NAME}
  ${FRAMEWORK_INCLUDES}
  ${FRAMEWORK_SOURCES}
  )


set(Assimp_FOUND false)
if(Assimp_FOUND)
  message("ahoj")
endif()

set(Internal_deps SDL2 GLEW glm imgui)
if(FreeImage_FOUND AND FreeImagePlus_FOUND)
	add_definitions(-DUSE_FREEIMAGE)
	set(Internal_deps ${Internal_deps} FreeImage FreeImagePlus)	
endif()
if(Assimp_FOUND)
	add_definitions(-DUSE_ASSIMP)
	set(Internal_deps ${Internal_deps} Assimp)	
endif()

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

