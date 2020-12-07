#Indicate the minimum version of CMake needed to use
#this CMakeLists.txt file.
cmake_minimum_required(VERSION 3.12)

# Declare the name of the project.
project(GFX_Programming VERSION 1.0 DESCRIPTION "Exam Autumn 2020.")

# Ask CMake to find the OpenGL package
find_package(OpenGL REQUIRED)

#https://stackoverflow.com/questions/45955272/modern-way-to-set-compiler-flags-in-cross-platform-cmake-project
set(COMPILER_WARNINGS_AND_ERRORS
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:MSVC>:/w44061>
    $<$<CXX_COMPILER_ID:MSVC>:/w44062>
    $<$<CXX_COMPILER_ID:MSVC>:/w44265>
    $<$<CXX_COMPILER_ID:MSVC>:/permissive->)


# Set a global output directory for libraries and runtime
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Instructs CMake to add the external subdirectories
add_subdirectory(external/glfw)
add_subdirectory(external/glew)
add_subdirectory(external/glm)
add_subdirectory(external/assimp)
add_subdirectory(external/openal)
add_subdirectory(external/stb_image)
add_subdirectory(external/audio_format)

# Instructs CMake to add the functionality available in the program
add_subdirectory(core)
add_subdirectory(rendering)
add_subdirectory(applications)