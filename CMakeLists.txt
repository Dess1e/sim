cmake_minimum_required (VERSION 3.0)
project (sim)

file(GLOB_RECURSE SHADERS Shaders/*.glsl)
file(GLOB_RECURSE SOURCES "Sources/*.cpp" "Sources/*.hpp" "Sources/*.h")

add_executable( ${PROJECT_NAME}
                ${SHADERS}
                ${SOURCES})
find_package(OpenGL REQUIRED)

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Build directory and source directory are the same" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Warning, source directory contains spaces" )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
        message( "Warning, build directory contains spaces" )
endif()

# Compile external dependencies 
add_subdirectory (ThirdParty)

include_directories(
        ThirdParty/imgui/
        ThirdParty/glfw/include/
        ThirdParty/glm/
        ThirdParty/glew/include/
        ThirdParty/assimp/include/
        ThirdParty/bullet/src/
        .
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
    assimp
    ImGUI
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

target_link_libraries(sim
	${ALL_LIBS}
)

add_custom_command(TARGET sim PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shaders/ $<TARGET_FILE_DIR:sim>/Shaders)
add_custom_command(TARGET sim PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Resources/ $<TARGET_FILE_DIR:sim>/Resources)
