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
    $<$<CONFIG:Debug>:FLAG_DEBUG>
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/engine/inclide
        ${PROJECT_SOURCE_DIR}/engine/external
)

add_subdirectory(engine)
target_link_libraries(${PROJECT_NAME} PUBLIC mattres)

set_target_properties(${PROJECT_NAME}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/res 
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)
