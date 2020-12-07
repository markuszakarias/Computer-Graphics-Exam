# Include libs from external
include_directories("${PROJECT_SOURCE_DIR}/external/assimp")
include_directories("${PROJECT_SOURCE_DIR}/external/openal")
include_directories("${PROJECT_SOURCE_DIR}/external/glew")
include_directories("${PROJECT_SOURCE_DIR}/external/glfw")
include_directories("${PROJECT_SOURCE_DIR}/external/glm")
include_directories("${PROJECT_SOURCE_DIR}/external/stb_image")
include_directories("${PROJECT_SOURCE_DIR}/external/audio_format")

file(GLOB EXAM_HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Exam/inc/*.h")
file(GLOB EXAM_SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Exam/src/*.cpp")

add_executable(exam "${EXAM_HEADER_FILES}" "${EXAM_SOURCE_FILES}" "Exam/Exam.cpp")

target_include_directories(exam PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Exam")
target_compile_options(exam PRIVATE ${COMPILER_WARNINGS_AND_ERRORS} ${COMPILER_SANITIZERS})
target_compile_definitions(exam PRIVATE GLEW_STATIC)

target_link_libraries(
    exam 
    PRIVATE libglew_static glfw glm assimp audio OpenAL stb_image OpenGL::GL core render)

add_custom_command(TARGET exam POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
                                                        ${PROJECT_SOURCE_DIR}/assets
                                                        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)

target_include_directories(exam PRIVATE "${PROJECT_SOURCE_DIR}/applications/Exam/inc" 
                                          "${PROJECT_SOURCE_DIR}/core/inc" 
                                          "${PROJECT_SOURCE_DIR}/rendering/inc")
