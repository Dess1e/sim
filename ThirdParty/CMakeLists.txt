if(MSVC AND NOT "${MSVC_VERSION}" LESS 1400)
	add_definitions( "/MP" )
endif()


add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

### GLFW ###

add_subdirectory (glfw)

include_directories(
	glfw/include/GLFW/
	glew/include/
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
set(OPENGL_LIBRARY
	${OPENGL_LIBRARY}
	-lGL -lGLU -lXrandr -lXext -lX11 -lrt
	${CMAKE_DL_LIBS}
	${GLFW_LIBRARIES}
)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
set(OPENGL_LIBRARY
	${OPENGL_LIBRARY}
	${CMAKE_DL_LIBS}
	${GLFW_LIBRARIES}
)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

### GLEW ###

set(GLEW_SOURCE
	glew/src/glew.c
)

set(GLEW_HEADERS
)


add_library( GLEW_1130 STATIC
	${GLEW_SOURCE}
	${GLEW_INCLUDE}
)

target_link_libraries(GLEW_1130
	${OPENGL_LIBRARY}
	${EXTRA_LIBS}
)


### ANTTWEAKBAR ###

set(IMGUI_SOURCES
imgui/imgui.cpp
imgui/imgui_demo.cpp
imgui/imgui_draw.cpp
imgui/examples/imgui_impl_glfw.cpp
imgui/examples/imgui_impl_opengl3.cpp)

set(IMGUI_HEADERS
imgui/imconfig.h
imgui/imgui.h
imgui/imgui_internal.h
imgui/stb_rect_pack.h
imgui/stb_textedit.h
imgui/stb_truetype.h
imgui/examples/imgui_impl_glfw.h
imgui/examples/imgui_impl_opengl3.h)

add_library(ImGUI ${IMGUI_SOURCES} ${IMGUI_HEADERS})

### ASSIMP ###
# AssImp already has a CMakeLists.txt so let's use these

# Compile built-in, modified version of Zlib
include(CheckIncludeFile)
include(CheckTypeSize)
include(CheckFunctionExists)
add_subdirectory( assimp/contrib/zlib )

# Compile without Boost
include_directories( assimp/code/BoostWorkaround )
add_definitions( -DASSIMP_BUILD_BOOST_WORKAROUND )

# Compile AssImp
set( LIB_INSTALL_DIR "lib")
set(LIBASSIMP_COMPONENT libassimp3.0-r1270-OGLtuts)
set(ZLIB_LIBRARIES zlib)
set(BUILD_STATIC_LIB ON)
#set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/zlib)
add_subdirectory( assimp/code )


### BULLET ###
# Bullet already has a CMakeLists.txt so let's use these

set(BULLET_VERSION 2.81)
include_directories(
	bullet/src
)
add_subdirectory( bullet/src/BulletSoftBody )
add_subdirectory( bullet/src/BulletCollision )
add_subdirectory( bullet/src/BulletDynamics )
add_subdirectory( bullet/src/LinearMath )


# stb

#set(STB_SOURCE stb/stb_image.h)
#target_include_directories(sim PUBLIC ${STB_SOURCE})
