#include "VulkanSetup.h"
#include <stdexcept>
#include <assert.h>
#include <set>

void VulkanSetup::initVulkan(int *status) {
    createInstance();
    getPhysicalDevice();
    createLogicalDevice();
    createUniversalComputeDescriptorSetLayout();
}

void VulkanSetup::createInstance()
{
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Volko Compute";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Compute Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Creation information for vkInstance
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // Instance extention list
    std::vector<const char*> instanceExtension = std::vector<const char*>();

    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;

    // Vk Instance
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("FAILED TO CREATE A VULKAN INSTANCE!!");
    }
}

void VulkanSetup::getPhysicalDevice() {
    // Enumerating physical devices the vkInstance can access
    uint32_t deviceCount{};
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("CANT FIND A GPU THAT SUPPORTS VULKAN!!");
    }

    // List of physical devices
    std::vector<VkPhysicalDevice> deviceList(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, deviceList.data());

    // Get the first device for now later on get the best one
    mainDevice.physicalDevice = deviceList[0];
}

QueueFamilyIndices VulkanSetup::getQueueFamilies(VkPhysicalDevice &device) {
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;

    assert(device != VK_NULL_HANDLE && "Invalid physical device handle!!");
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    assert(queueFamilyCount > 0 && "Queue family count should be greater than 0!!");

    std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyList.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilyList) {
        if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)) {
            indices.computeFamily = i;
        }
        if (indices.isValid()) {
            break;
        }
        i += 1;
    }
    return indices;
}

void VulkanSetup::createLogicalDevice() {
    QueueFamilyIndices indices = getQueueFamilies(mainDevice.physicalDevice);
    
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.computeFamily.value();
    queueCreateInfo.queueCount = 1;
    float priority = 1.0f; // 1 is heighest
    queueCreateInfo.pQueuePriorities = &priority;

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = 1; // Just the compute queue here
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtentions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtentions.data();

    // Physical device features the logical device will be using...
    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

    VkResult result = vkCreateDevice(mainDevice.physicalDevice, &deviceCreateInfo, nullptr, &mainDevice.logicalDevice);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("UNABLE TO CREATE A LOGICAL DEVICE!!");
    }

    // Queues have to be created at the same time as the device creation
    vkGetDeviceQueue(mainDevice.logicalDevice, indices.computeFamily.value(), 0, &computeQueue);
}

// Created and called only once unlike computedescriptorSet
void VulkanSetup::createUniversalComputeDescriptorSetLayout() {
    std::vector<VkDescriptorSetLayoutBinding> layoutBindings;

    // Uniform Buffer Binding. Used for making uniform data which is accessible to shaders through Uniform Buffer Object.
    VkDescriptorSetLayoutBinding uniformBufferBinding = {};
    uniformBufferBinding.binding = 0; // See the GLSI of it
    uniformBufferBinding.descriptorCount = MAX_UNIFORM_BUFFERS;
    uniformBufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uniformBufferBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
    layoutBindings.push_back(uniformBufferBinding);

    // Storage Buffers Bindings (Generic)
    for (uint32_t i = 1; i < MAX_STORAGE_BUFFERS; i++) {
        VkDescriptorSetLayoutBinding storageBufferBinding = {};
        storageBufferBinding.binding = 1;
        uniformBufferBinding.descriptorCount = MAX_STORAGE_BUFFERS;
        uniformBufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        uniformBufferBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
        layoutBindings.push_back(storageBufferBinding);
    }

    // Descriptor set layout
    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
    layoutInfo.pBindings = layoutBindings.data();
    
    VkResult result = vkCreateDescriptorSetLayout(mainDevice.logicalDevice, &layoutInfo, nullptr, &computeDescriptorSetLayout);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("UNABLE TO CREATE DESCRIPTOR SET LAYOUT!!");
    }
}
