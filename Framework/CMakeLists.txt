set(LIB_NAME Framework)

set(HEADER_PATH ${FitGL_SOURCE_DIR}/${LIB_NAME})

set(FRAMEWORK_INCLUDES
	BaseApp.h
	SDLWindow/SDLEventProc.h
	SDLWindow/SDLWindow.h
	Loader.h
	Gui.h
	Mesh.h
	Image.h
	Node.h
	Material.h
	Camera.h
	Manipulator.h
	Timer.h
	utils/utils.h
  
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
	Camera.cpp
	Manipulator.cpp

	utils/drawNode.cpp
  )

add_library(${LIB_NAME}
  ${FRAMEWORK_INCLUDES}
  ${FRAMEWORK_SOURCES}
  )

set(Internal_deps SDL2 geGL glm imgui)
if(FreeImage_FOUND AND FreeImagePlus_FOUND)
	add_definitions(-DUSE_FREEIMAGE)
	set(Internal_deps ${Internal_deps} FreeImage FreeImagePlus)	
endif()
if(assimp_FOUND)
	add_definitions(-DUSE_ASSIMP)
	include_directories(${ASSIMP_INCLUDE_DIR})	
	set(Internal_deps ${Internal_deps} assimp)	
endif()

set(Internal_inc ${FitGL_SOURCE_DIR}/Framework)
set(includes_to_export ${FitGL_SOURCE_DIR}/Framework)

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

