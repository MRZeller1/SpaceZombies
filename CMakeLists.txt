cmake_minimum_required(VERSION 3.20)

project(SpaceZombies)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

# Add executable
file(GLOB SOURCES "src/*.cpp")
add_executable(SpaceZombies ${SOURCES})

# If you're using the static library
target_link_libraries(SpaceZombies "${PROJECT_SOURCE_DIR}/lib/libraylib.a")

# Link against macOS frameworks
if(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    find_library(OPENGL_LIBRARY OpenGL)
    target_link_libraries(SpaceZombies
        "${COCOA_LIBRARY}"
        "${IOKIT_LIBRARY}"
        "${COREVIDEO_LIBRARY}"
        "${OPENGL_LIBRARY}"
    )
endif()

# Copy resources directory to the build directory
add_custom_command(TARGET SpaceZombies POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:SpaceZombies>/resources)

# Enable warnings and treat some as errors, but allow for common issues
target_compile_options(SpaceZombies PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Werror=return-type
    -Werror=uninitialized
    -Wno-missing-braces
    -Wno-unused-parameter
    -Wno-unused-variable
    -Wno-reorder
)
