cmake_minimum_required(VERSION 3.21)
project(CG LANGUAGES C CXX)

find_package(OpenGL REQUIRED)
find_package(FreeGLUT CONFIG REQUIRED)

add_executable(simple main.c)
set_target_properties(simple PROPERTIES C_STANDARD 17 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
target_link_libraries(simple PRIVATE
    $<IF:$<TARGET_EXISTS:FreeGLUT::freeglut>,FreeGLUT::freeglut,FreeGLUT::freeglut_static>
    OpenGL::GL
)
if(MSVC)
    target_compile_options(simple PRIVATE /utf-8)
endif()
