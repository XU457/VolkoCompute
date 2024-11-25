#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

struct QueueFamilyIndices {
    std::optional<int> computeFamily;
    bool isValid() const {
        return computeFamily.has_value();
    }
};

const std::vector<const char*> deviceExtentions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

constexpr uint32_t MAX_STORAGE_BUFFERS = 8;
constexpr uint32_t MAX_UNIFORM_BUFFERS = 1;

class VulkanSetup {
public:
    VulkanSetup();

    void initVulkan(int *status);
    void createBuffer(VkBuffer* bufferObj, int * status);
    void allocateMemory();

    ~VulkanSetup();

private:
    VkInstance instance;
    struct {
        VkPhysicalDevice physicalDevice;
        VkDevice logicalDevice;
    } mainDevice;

    VkQueue computeQueue;
    std::vector<VkCommandBuffer> computeCommandBuffers;
    VkPipeline computePipeline;
    VkDescriptorSetLayout computeDescriptorSetLayout;
    VkPipelineLayout computePipelineLayotut;
    VkCommandPool commandPool;
    std::vector<VkDescriptorSet> computeDescriptorSets;
    std::vector<VkSemaphore> computeFinishedSemphore;
    std::vector<VkFence> computeInFlightFences;

    // Vulkan Functions
    // - Create functions
    void createInstance();
    void createLogicalDevice();
    void createUniversalComputeDescriptorSetLayout();

    // - Get functions
    void getPhysicalDevice();
    QueueFamilyIndices getQueueFamilies(VkPhysicalDevice& device);
    
};