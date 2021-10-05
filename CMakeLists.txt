cmake_minimum_required(VERSION 3.0.0)
project(SDL_imgui_sample VERSION 1.0.0)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

find_package(SDL2 REQUIRED)
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)

set(source_directory src)
set(imgui_source_directory ${source_directory}/imgui)

set(main_sources ${source_directory}/main.cpp)

set(imgui_library_sources
    ${imgui_source_directory}/imgui.cpp
    ${imgui_source_directory}/imgui_draw.cpp
    ${imgui_source_directory}/imgui_tables.cpp
    ${imgui_source_directory}/imgui_widgets.cpp
    # SDL support.
    ${imgui_source_directory}/backends/imgui_impl_sdl.cpp
    # OpenGL support.
    ${imgui_source_directory}/backends/imgui_impl_opengl3.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${main_sources})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    ${SDL2_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR})

add_library(imgui
    ${imgui_library_sources})

target_include_directories(imgui PUBLIC
    ${SDL2_INCLUDE_DIRS}
    ${imgui_source_directory} ${imgui_source_directory}/backends)
# Configure Imgui to use OpengGL through SDL2.  GL_GLEXT_PROTOTYPES=1 is an option for Linux.
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_CUSTOM=<SDL_opengl.h> GL_GLEXT_PROTOTYPES=1 IMGUI_IMPL_OPENGL_ES3)

target_link_libraries(
    ${CMAKE_PROJECT_NAME} PUBLIC
    ${SDL2_LIBRARIES}
    OpenGL::GL
    imgui
)

# Copy required font file to build directory.
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/imgui/misc/fonts/DroidSans.ttf ${CMAKE_CURRENT_BINARY_DIR}/DroidSans.ttf COPYONLY)
