#TODO: Comment and remove unnecessary code

file(GLOB RENDER_HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h")
file(GLOB RENDER_SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(render ${RENDER_SOURCE_FILES} ${RENDER_HEADER_FILES})

target_compile_options(render PRIVATE ${COMPILER_WARNINGS_AND_ERRORS} ${COMPILER_SANITIZERS})
target_compile_definitions(render PUBLIC _USE_MATH_DEFINES)
target_include_directories(render PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/inc")

include_directories("${PROJECT_SOURCE_DIR}/external/assimp")
include_directories("${PROJECT_SOURCE_DIR}/external/openal")
include_directories("${PROJECT_SOURCE_DIR}/external/glew")
include_directories("${PROJECT_SOURCE_DIR}/external/glfw")
include_directories("${PROJECT_SOURCE_DIR}/external/glm")
include_directories("${PROJECT_SOURCE_DIR}/external/stb_image")
include_directories("${PROJECT_SOURCE_DIR}/external/audio_format")

target_link_libraries(
    render 
    PRIVATE libglew_static glfw glm assimp audio OpenAL stb_image OpenGL::GL)

source_group(TREE "${PROJECT_SOURCE_DIR}/rendering/inc" PREFIX "Header Files" FILES ${RENDER_HEADER_FILES})
source_group(TREE "${PROJECT_SOURCE_DIR}/rendering/src" PREFIX "Source Files" FILES ${RENDER_SOURCE_FILES})

target_include_directories(render PRIVATE "${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/core/inc")
