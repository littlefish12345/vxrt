cmake_minimum_required(VERSION 3.15)

project(vxrt)
set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(3rdparty/glad)
add_subdirectory(3rdparty/glfw)
add_subdirectory(3rdparty/glm)

aux_source_directory("${PROJECT_SOURCE_DIR}/src" VXRT_ALL_SOURCE)

add_custom_target(generate_shader_string ALL
    COMMAND ${CMAKE_COMMAND} -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}" -P "${PROJECT_SOURCE_DIR}/shader_string.cmake"
)

add_executable(vxrt ${VXRT_ALL_SOURCE})

target_include_directories(vxrt PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(vxrt PUBLIC "${PROJECT_SOURCE_DIR}/shaders/include")

target_link_libraries(vxrt OpenGL::GL)
target_link_libraries(vxrt glfw)
target_link_libraries(vxrt glad)
target_link_libraries(vxrt glm)
