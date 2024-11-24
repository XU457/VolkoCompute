# Volko Compute
Vulkan backend to emulate cuda like environment 

## Build Shared Library:
mkdir build && cd build
cmake -DBUILD_SHARED_LIBS=ON ..
cmake --build .

## Build Static Library:
mkdir build && cd build
cmake -DBUILD_SHARED_LIBS=OFF ..
cmake --build .

## Install:
cmake --install .

## Integration:
install(EXPORT NeuralNetworkVulkanTargets
    FILE NeuralNetworkVulkanConfig.cmake
    DESTINATION lib/cmake/NeuralNetworkVulkan
)