cmake_minimum_required(VERSION 3.10)
project(LibraryManagementSystem CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect source files
file(GLOB SOURCES "src/*.cpp")

add_executable(library ${SOURCES})

target_include_directories(library PRIVATE src)

# Ensure data directory exists at build time
add_custom_command(TARGET library POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:library>/data
    COMMENT "Creating data directory..."
)
