cmake_minimum_required(VERSION 3.8)
project(NimbleRenderer)
# Project Version
set(PROJECT_VERSION "0.0.1")

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)

# Set other flags
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Setting up options
option(BUILD_EXAMPLES "Build the examples" OFF)
option(DOXYGEN_ACTIVE "Document with Doxygen" OFF)

if(DOXYGEN_ACTIVE)
  find_package(Doxygen QUIET)
  if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/docs/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(doxygen-docs ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
  endif()
endif()

# Organize source files into "Source Files"
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "include/*.h")
file(GLOB_RECURSE SHADER_FILES "shaders/*.glsl")
include_directories(shaders)
include_directories(external/stb/)
include_directories(external/glm/)

# Setting compile definitions
add_compile_definitions(VERSION="${PROJECT_VERSION}")
add_compile_definitions($<$<CONFIG:Debug>:DEBUG>)

# Adding Glad
set(GLAD_SRC ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/src/glad.c)
set(GLAD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include)

# Create static library
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES} ${GLAD_SRC} ${SHADER_FILES})

# Include directory for external usage
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${GLAD_INCLUDE_DIR})

# Find OpenGL and GLFW
find_package(OpenGL REQUIRED)
add_subdirectory(./external/glfw-3.4 EXCLUDE_FROM_ALL)
add_subdirectory(./external/glm EXCLUDE_FROM_ALL)
if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC OpenGL::GL glfw)

source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Header Files" FILES ${HEADER_FILES})
source_group("External" FILES ${GLAD_SRC})
source_group("Shaders" FILES ${SHADER_FILES})

# Install headers and library
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

# Check if the generator is Visual Studio
if (CMAKE_GENERATOR MATCHES "Visual Studio")
  message(STATUS "Setting up for Visual Studio...")
  # Set the Working Directory for each Config 
  set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
elseif(CMAKE_GENERATOR MATCHES "Ninja")
  if(WIN32)
    # Copy compile_commands.json
    add_custom_target(
        copy_compile_commands ALL
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_SOURCE_DIR}/compile_commands.json
        DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
    )
  else()
    # symlink compile_commands.json
    add_custom_target(
        copy_compile_commands ALL
        COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_SOURCE_DIR}/compile_commands.json
        DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
    )
  endif()
endif()
