cmake_minimum_required (VERSION 3.0)
project (sim)

file(GLOB SHADERS shaders/*.glsl)
file(GLOB_RECURSE SRC "src/*.cpp" "src/*.hpp")
add_executable( ${PROJECT_NAME}
                ${SHADERS}
                ${SRC} )
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
add_subdirectory (lib)
include_directories(
        lib/imgui/
        lib/glfw-3.1.2/include/
        lib/glm-0.9.7.1/
        lib/glew-1.13.0/include/
        lib/assimp-3.0.1270/include/
        lib/bullet-2.81-rev2613/src/
        .
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
    GLEW_1130
    ImGui
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
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:sim>/shaders)
add_custom_command(TARGET sim PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/textures $<TARGET_FILE_DIR:sim>/textures)
