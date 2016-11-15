set(LIB_NAME NeiVu)

set(Internal_deps SDL2 glm vulkan Framework)

foreach(dep ${Internal_deps})
  if(NOT TARGET ${dep})
    message(${LIB_NAME} " Missing target: " ${dep})
  endif()
endforeach(dep)

if(assimp_FOUND)
	add_definitions(-DUSE_ASSIMP)
	include_directories(${ASSIMP_INCLUDE_DIR})	
	set(Internal_deps ${Internal_deps} assimp)	
endif()

set(HEADER_PATH ${FitGL_SOURCE_DIR}/${LIB_NAME})

set(FRAMEWORK_INCLUDES
  VuApp.h
  VuInfo.h
  VuDebug.h
  Context.h
  Pipeline.h
  Swapchain.h
  Texture.h
  Buffer.h
  VuMesh.h
  VuNode.h
  LoaderAssimp.h
  VuMaterial.h
)

set(FRAMEWORK_SOURCES
  VuApp.cpp
  VuInfo.cpp
  Context.cpp
  Pipeline.cpp
  Swapchain.cpp
  Texture.cpp
  Buffer.cpp
  VuMesh.cpp
  VuNode.cpp
  LoaderAssimp.cpp
  VuMaterial.cpp
)

add_library(${LIB_NAME}
  ${FRAMEWORK_INCLUDES}
  ${FRAMEWORK_SOURCES}
  )

set(Internal_inc ${FitGL_SOURCE_DIR}/NeiVu)

set(includes_to_export ${FitGL_SOURCE_DIR}/NeiVu)

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

