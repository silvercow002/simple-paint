cmake_minimum_required(VERSION 3.21)
project(CG
    LANGUAGES C CXX
    DESCRIPTION "HW 1"
)

find_package(OpenGL REQUIRED)
find_package(FreeGLUT CONFIG REQUIRED)

file(GLOB SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)
add_executable(simple ${SOURCES})

set_target_properties(simple PROPERTIES C_STANDARD 17 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)

target_include_directories(simple PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_link_libraries(simple PRIVATE
    $<IF:$<TARGET_EXISTS:FreeGLUT::freeglut>,FreeGLUT::freeglut,FreeGLUT::freeglut_static>
    OpenGL::GL
    OpenGL::GLU
)
if(MSVC)
    target_compile_options(simple PRIVATE /utf-8)
endif()
