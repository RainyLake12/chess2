cmake_minimum_required(VERSION 3.10)
project(chess2)

# Enable all warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# Recursively find all .cpp files in src/
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

# Add the executable
add_executable(chess2 ${SOURCE_FILES})

# Add the 'headers' directory to the include path
target_include_directories(chess2 PRIVATE headers)
