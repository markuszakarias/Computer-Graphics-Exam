# Include libs from external
include_directories("${PROJECT_SOURCE_DIR}/external/assimp")
include_directories("${PROJECT_SOURCE_DIR}/external/openal")
include_directories("${PROJECT_SOURCE_DIR}/external/glew")
include_directories("${PROJECT_SOURCE_DIR}/external/glfw")
include_directories("${PROJECT_SOURCE_DIR}/external/glm")
include_directories("${PROJECT_SOURCE_DIR}/external/stb_image")
include_directories("${PROJECT_SOURCE_DIR}/external/audio_format")

#================ Skybox Test Scene - Start ================# 
file(GLOB SKYBOX_HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/exam/inc/*.h")
file(GLOB SKYBOX_SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/exam/src/*.cpp")

add_executable(skybox "${SKYBOX_HEADER_FILES}" "${SKYBOX_SOURCE_FILES}" "exam/app.cpp")

target_include_directories(skybox PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/exam")
target_compile_options(skybox PRIVATE ${COMPILER_WARNINGS_AND_ERRORS} ${COMPILER_SANITIZERS})
target_compile_definitions(skybox PRIVATE GLEW_STATIC)

target_link_libraries(
    skybox 
    PRIVATE libglew_static glfw glm assimp audio OpenAL stb_image OpenGL::GL core render)

add_custom_command(TARGET skybox POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
                                                        ${PROJECT_SOURCE_DIR}/assets
                                                        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)

target_include_directories(skybox PRIVATE "${PROJECT_SOURCE_DIR}/applications/Skybox/inc" 
                                          "${PROJECT_SOURCE_DIR}/core/inc" 
                                          "${PROJECT_SOURCE_DIR}/rendering/inc")

#================ Skybox Test Scene - End ================# 