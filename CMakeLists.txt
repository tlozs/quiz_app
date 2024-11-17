cmake_minimum_required(VERSION 3.5.0)
project(quiz_app VERSION 0.1.0 LANGUAGES C)

# Set the C standard to ANSI
set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add all source files
file(GLOB SOURCES "source/*.c")

add_executable(quiz_app ${SOURCES})

# Set the output directory for the executable
set_target_properties(quiz_app PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/..")

# Enable all warnings and treat them as errors
target_compile_options(quiz_app PRIVATE -Wall -Wextra -Werror -pedantic)
