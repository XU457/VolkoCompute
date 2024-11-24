cmake_minimum_required(VERSION 3.10)
project(VolkoCompute VERSION 1.0.0)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Vulkan
find_package(Vulkan REQUIRED)

# Option to build shared or static library
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)  # Set ON for shared, OFF for static

# Create the library (STATIC or SHARED based on the option)
if(BUILD_SHARED_LIBS)
    add_library(VolkoCompute SHARED
        src/VulkanNN.cpp
        src/MatrixMultiplication.cpp
        src/Activations.cpp
        src/VulkanSetup.cpp
        src/Utils.cpp
    )
else()
    add_library(VolkoCompute STATIC
        src/VulkanNN.cpp
        src/MatrixMultiplication.cpp
        src/Activations.cpp
        src/VulkanSetup.cpp
        src/Utils.cpp
    )
endif()

# Include the public headers
target_include_directories(VolkoCompute PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Link Vulkan library
target_link_libraries(VolkoCompute Vulkan::Vulkan)

# Shader Compilation
file(GLOB_RECURSE SHADER_FILES shaders/*.comp)

set(SPIRV_FILES "")
foreach(SHADER ${SHADER_FILES})
    get_filename_component(FILE_NAME ${SHADER} NAME_WE)
    set(SPIRV "${CMAKE_BINARY_DIR}/shaders/${FILE_NAME}.spv")
    add_custom_command(
        OUTPUT ${SPIRV}
        COMMAND ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} -V ${SHADER} -o ${SPIRV}
        DEPENDS ${SHADER}
        COMMENT "Compiling ${SHADER} to SPIR-V"
    )
    list(APPEND SPIRV_FILES ${SPIRV})
endforeach()

add_custom_target(Shaders DEPENDS ${SPIRV_FILES})

# Ensure shaders are built before the library
add_dependencies(VolkoCompute Shaders)

# Install rules (for library, headers, and shaders)
install(TARGETS VolkoCompute
    EXPORT VolkoComputeTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# Install public headers
install(DIRECTORY include/ DESTINATION include)

# Install shaders (compiled SPIR-V binaries)
install(DIRECTORY ${CMAKE_BINARY_DIR}/shaders/ DESTINATION shaders)

# Export Configuration
install(EXPORT VolkoComputeTargets
    FILE VolkoComputeConfig.cmake
    DESTINATION lib/cmake/VolkoCompute
)

