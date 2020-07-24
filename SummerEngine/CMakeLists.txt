# CMakeList.txt : CMake project for SummerEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

# Find SDL2 and OpenGL
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")

find_package(SDL2 REQUIRED COMPONENTS main)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

# Add source to this project's executable.
add_executable (SummerEngine "SummerEngine.cpp" "SummerEngine.h" "stb_image.cpp" "stb_image.h" "ShaderUtils.cpp" "ShaderUtils.h")
target_link_libraries(SummerEngine ${SDL2_LIBS} ${GLEW_LIBRARY} ${OPENGL_LIBRARY})

add_definitions( -DROOT_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}\" )

# Copy SDL2 DLLs to output folder on Windows
if(WIN32)
    foreach(DLL ${SDL2_DLLS} ${GLEW_DLL})
        add_custom_command(TARGET SummerEngine POST_BUILD COMMAND
            ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:SummerEngine>)
    endforeach()
endif()
