cmake_minimum_required(VERSION 3.21)
project(CG VERSION 0.1.0 LANGUAGES C)

add_executable(simple src/main.c src/draw.c src/menu.c)
set_target_properties(simple PROPERTIES
    C_STANDARD 17 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
target_include_directories(simple PRIVATE include)

if(WIN32)
    find_package(OpenGL REQUIRED)
    find_package(FreeGLUT CONFIG REQUIRED)
    if(TARGET FreeGLUT::freeglut)
        target_link_libraries(simple PRIVATE FreeGLUT::freeglut)
    else()
        target_link_libraries(simple PRIVATE FreeGLUT::freeglut_static)
    endif()
    target_link_libraries(simple PRIVATE OpenGL::GL OpenGL::GLU)
else()
    # Use the same Mesa/X11 stack for GL, GLU and FreeGLUT on macOS.
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GRAPHICS REQUIRED IMPORTED_TARGET glut gl glu)
    target_link_libraries(simple PRIVATE PkgConfig::GRAPHICS m)
endif()

if(MSVC)
    target_compile_options(simple PRIVATE /utf-8)
endif()

install(TARGETS simple RUNTIME DESTINATION bin)
if(WIN32)
    install(FILES $<TARGET_RUNTIME_DLLS:simple> DESTINATION bin)
    include(InstallRequiredSystemLibraries)
    # Include FreeGLUT's license when using vcpkg, including local prefix builds.
    install(FILES "${FreeGLUT_DIR}/copyright" DESTINATION licenses
            RENAME freeglut.txt OPTIONAL)
endif()

set(CPACK_PACKAGE_NAME "CG")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
if(WIN32)
    set(CPACK_GENERATOR ZIP)
else()
    set(CPACK_GENERATOR TGZ)
endif()
include(CPack)
