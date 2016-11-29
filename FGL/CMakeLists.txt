set(LIB_NAME FGL)

set(deps SDL2 geGL glm imgui)

if(TARGET FreeImage AND TARGET FreeImagePlus)
	add_definitions(-DUSE_FREEIMAGE)
	set(deps ${deps} FreeImage FreeImagePlus)	
endif()

if(TARGET assimp)
	add_definitions(-DUSE_ASSIMP)
	include_directories(${ASSIMP_INCLUDE_DIR})	
	set(deps ${deps} assimp)	
endif()

foreach(dep ${deps})
  if(NOT TARGET ${dep})
    message(${LIB_NAME} " Missing target: " ${dep})
	return()
  endif()
endforeach(dep)

set(HEADER_PATH ${FitGL_SOURCE_DIR}/${LIB_NAME})

set(includes
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
	Shared.h
	Shape.h
	Geometry.h   
	geGLshared.h
  )

set(sources
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
	Shape.cpp
	Geometry.cpp 
	utils/drawNode.cpp
  )

add_library(${LIB_NAME}
  ${includes}
  ${sources}
  )

set(include_dir ${HEADER_PATH})
set(include_dir_export ${HEADER_PATH})

target_link_libraries(${LIB_NAME} ${deps})
set_target_properties(${LIB_NAME} PROPERTIES
  INCLUDE_DIRECTORIES "${include_dir}"
  INTERFACE_INCLUDE_DIRECTORIES "${include_dir_export}"
  )

install(FILES
  ${includes_export} DESTINATION include/${LIB_NAME}
  )

install(TARGETS ${LIB_NAME}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  )

