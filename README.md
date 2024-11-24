# Volko Compute
Vulkan backend to emulate cuda like environment 

## Prerequisites

Before building `VolkoCompute`, ensure you have the following installed:

- **CMake** (for building the project)
- **Vulkan SDK** (for Vulkan backend)
  - Download and install from the [Vulkan SDK](https://vulkan.lunarg.com/sdk/home)
- A **C++ compiler** that supports C++11 or later

## Build Instructions

### Build Shared Library
mkdir build && cd build\
cmake -DBUILD_SHARED_LIBS=ON ..\
cmake --build .

### Build Static Library:
mkdir build && cd build\
cmake -DBUILD_SHARED_LIBS=OFF ..\
cmake --build .

### Install:
cmake --install .

### Integration:
install(EXPORT VolkoComputeTargets\
    &nbsp;&nbsp;FILE VolkoComputeConfig.cmake\
    &nbsp;&nbsp;DESTINATION lib/cmake/VolkoCompute\
)