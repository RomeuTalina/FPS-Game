cmake_minimum_required(VERSION 3.10)
project(Aim LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.0
)
FetchContent_MakeAvailable(raylib)

add_executable(Aim src/main.c)
target_link_libraries(Aim PRIVATE raylib)

if(WIN32)
    target_link_libraries(Aim PRIVATE opengl32 gdi32 winmm)
elseif (APPLE)
  find_library(COCOA_LIBRARY Cocoa)
  find_library(IOKIT_LIBRARY IOKit)
  target_link_libraries(Aim PRIVATE ${COCOA_LIBRARY} ${IOKIT_LIBRARY})
elseif (UNIX)  # Linux
  target_link_libraries(Aim PRIVATE m pthread dl)
endif()
