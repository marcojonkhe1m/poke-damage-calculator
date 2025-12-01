# Required CMake version
cmake_minimum_required(VERSION 3.10)

# Project name and version
set(PROJECT_NAME Pokemon-damage-calculator)
project(${PROJECT_NAME} VERSION 1.0)

# Set C++ version required
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Output directory for the binary
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Export compile_commands.json for lsp
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT EXISTS "${CMAKE_SOURCE_DIR}/compile_commands.json")
    message(STATUS "Creating symlink for compile_commands.json in project root")

    execute_process(
	COMMAND ${CMAKE_COMMAND} -E create_symlink
	${CMAKE_BINARY_DIR}/compile_commands.json
	${CMAKE_SOURCE_DIR}/compile_commands.json
    )
endif()

# Include include directory - holding header files
include_directories(include)

# Include Curses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Add subdirectories
add_subdirectory(src)

# Link SDL2 library to the execustable
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})




