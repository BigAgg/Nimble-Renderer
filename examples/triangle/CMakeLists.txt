cmake_minimum_required(VERSION 3.8)
project(Triangle)

add_executable(${PROJECT_NAME} main.cpp)

# Ensure this example rebuilds when Renderer changes
add_dependencies(${PROJECT_NAME} NimbleRenderer)

# Check if the generator is Visual Studio
if (CMAKE_GENERATOR MATCHES "Visual Studio")
  message(STATUS "Setting up for Visual Studio...")
  # Set the Working Directory for each Config 
  set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>")

  # Set the startup project for Visual Studio
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

# Link to the Renderer library
target_link_libraries(${PROJECT_NAME} PRIVATE NimbleRenderer)

# set the source directory of resources
set(RES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/res)

# Copy the contents of the res folder to the build directory if there is one
if(EXISTS ${RES_DIR})
	message(STATUS "Resource folder found: ${RES_DIR}")
	add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${RES_DIR} $<TARGET_FILE_DIR:${PROJECT_NAME}>
		COMMENT "Copying resources to the build directory..."
	)
else()
	# Skipping copying resources
	message(INFO "No Resources for '${PROJECT_NAME}' to copy")
endif()