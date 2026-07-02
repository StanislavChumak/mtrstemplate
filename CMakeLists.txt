cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)

project(mtrstemplate VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
endif()

add_executable(${PROJECT_NAME} 
src/main.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Release>:FLAG_RELEASE>
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/engine/inclide
        ${PROJECT_SOURCE_DIR}/engine/external
)

add_subdirectory(engine)
target_link_libraries(${PROJECT_NAME} PUBLIC mattres)

add_subdirectory(asset_compiler mtrsc)
add_custom_target(run_mtrsc ALL
    COMMAND $<TARGET_FILE:mtrsc> ${PROJECT_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/bin/res
    DEPENDS mtrsc
    COMMENT "Building mtrsc (if needed) and generating runtime files"
)
add_dependencies(${PROJECT_NAME} run_mtrsc)

set_target_properties(${PROJECT_NAME}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
)
