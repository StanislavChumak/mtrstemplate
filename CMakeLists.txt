cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)

project(fractal_factory VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
endif()

# The game describes itself to the engine. The engine builds the application
# executable when MATTRESS_BUILD_APP is ON, otherwise it is just a library.
set(MATTRESS_BUILD_APP ON)
set(MATTRESS_APP_NAME "mtrstemplate")

set(ENGINE_WINDOW_NAME         "Mattress Template" CACHE STRING "Window title")
set(ENGINE_PACK_DIR            "res/"              CACHE STRING "Runtime pack directory")
set(ENGINE_SCENE_DIR           "res/"              CACHE STRING "Runtime scene directory")
set(ENGINE_START_SCENE         "main"              CACHE STRING "Scene loaded on startup")
set(ENGINE_ICON_PATHS          ""                  CACHE STRING "Window icons (CMake list), relative to the executable")
set(ENGINE_ICON_COUNT          0                   CACHE STRING "Count of window icons")
set(ENGINE_WINDOW_SIZE_X       256                 CACHE STRING "Window width in points")
set(ENGINE_WINDOW_SIZE_Y       160                 CACHE STRING "Window height in points")
set(ENGINE_START_POINT_SIZE    4                   CACHE STRING "Pixel scale")
set(ENGINE_MAX_FPS             60                  CACHE STRING "Target frame rate")
set(ENGINE_PACKS_CACHE_LIMIT   15                  CACHE STRING "Packs cache limit")
set(ENGINE_SCENES_CACHE_LIMIT  50                  CACHE STRING "Scenes cache limit")
set(ENGINE_SOUND_LOCATION_SCALE 10                 CACHE STRING "Sound location scale")
set(ENGINE_DISPLAY_CURSOR      OFF                 CACHE BOOL   "Display the cursor")
set(ENGINE_BLEND               ON                  CACHE BOOL   "Enable blending")
set(ENGINE_FIXED_HORIZONTAL    ON                  CACHE BOOL   "Fix horizontal camera side")
set(ENGINE_FIXED_VERTICAL      ON                  CACHE BOOL   "Fix vertical camera side")
set(ENGINE_CLEAR_COLOR_R       0.0                 CACHE STRING "Clear color red")
set(ENGINE_CLEAR_COLOR_G       0.0                 CACHE STRING "Clear color green")
set(ENGINE_CLEAR_COLOR_B       0.0                 CACHE STRING "Clear color blue")

add_subdirectory(engine)

if(NOT MATTRESS_APP_TARGET)
    message(FATAL_ERROR "MATTRESS_BUILD_APP is OFF but this game has no main()")
endif()

if(CMAKE_BUILD_TYPE AND (NOT DEFINED MTRSC_SCRIPTS_CONFIG OR MTRSC_SCRIPTS_CONFIG STREQUAL "Debug"))
    set(MTRSC_SCRIPTS_CONFIG "${CMAKE_BUILD_TYPE}" CACHE STRING "Scripts build type" FORCE)
endif()

add_subdirectory(asset_compiler mtrsc)
file(GENERATE OUTPUT ${CMAKE_BINARY_DIR}/mtrsc/scripts_build/engine_dir.cmake
    CONTENT "set(ENGINE_DIR \"${PROJECT_SOURCE_DIR}/engine\")
set(EXTERNAL_INCLUDE_DIR \"${PROJECT_SOURCE_DIR}/engine/external\")"
)
add_custom_target(run_mtrsc ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/asset_compiler/scripts_build"
        "${CMAKE_BINARY_DIR}/mtrsc/scripts_build"
    COMMAND ${CMAKE_COMMAND} -E env
        "MTRSC_SCRIPTS_CONFIG=${MTRSC_SCRIPTS_CONFIG}"
        "CMAKE_GENERATOR=${CMAKE_GENERATOR}"
        "CMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}"
        "CMAKE_C_COMPILER=${CMAKE_C_COMPILER}"
        "CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
        $<TARGET_FILE:mtrsc> ${PROJECT_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/bin/res
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/mtrsc
    DEPENDS mtrsc
    COMMENT "Building mtrsc (if needed) and generating runtime files"
)
add_dependencies(${MATTRESS_APP_TARGET} run_mtrsc)
