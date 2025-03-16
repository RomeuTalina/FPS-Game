cmake_minimum_required(VERSION 3.10)
project(Game LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.0
)
FetchContent_MakeAvailable(raylib)

# List all your source files (adjust paths as needed)
set(SOURCES
    src/main.c
    src/raygui_impl.c  # This file defines RAYGUI_IMPLEMENTATION and includes raygui.h
    src/player.c
    src/target.c
    src/cJSON.c
    src/settings.c
)

add_executable(Game ${SOURCES})
target_link_libraries(Game PRIVATE raylib)

if(WIN32)
    target_link_libraries(Game PRIVATE opengl32 gdi32 winmm)
elseif(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    target_link_libraries(Game PRIVATE ${COCOA_LIBRARY} ${IOKIT_LIBRARY})
elseif(UNIX)  # Linux
    target_link_libraries(Game PRIVATE m pthread dl)
endif()

# Optionally add the source directory to the include paths
target_include_directories(Game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
