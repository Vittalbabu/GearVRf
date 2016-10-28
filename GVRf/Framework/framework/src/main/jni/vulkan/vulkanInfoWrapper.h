

#ifndef FRAMEWORK_VULKANINFOWRAPPER_H
#define FRAMEWORK_VULKANINFOWRAPPER_H

#include <vulkan/vulkan.h>

namespace gvr {
struct GVR_VK_SwapchainBuffer
{
    VkImage image;
    VkCommandBuffer cmdBuffer;
    VkImageView view;
    VkDeviceSize size;
    VkDeviceMemory mem;
    VkBuffer buf;
};

struct GVR_VK_DepthBuffer {
    VkFormat format;
    VkImage image;
    VkDeviceMemory mem;
    VkImageView view;
};

struct GVR_VK_Vertices {
    VkBuffer buf;
    VkDeviceMemory mem;
    VkPipelineVertexInputStateCreateInfo vi;
       VkVertexInputBindingDescription      vi_bindings[6];
        VkVertexInputAttributeDescription    vi_attrs[6];

 //   VkVertexInputBindingDescription*      vi_bindings;
 //   VkVertexInputAttributeDescription*    vi_attrs;
};

struct GVR_Uniform {
    VkBuffer buf;
    VkDeviceMemory mem;
    VkDescriptorBufferInfo bufferInfo;
    VkDeviceSize allocSize;
};

struct OutputBuffer
{
    VkBuffer imageOutputBuffer;
    VkDeviceMemory memory;
    VkDeviceSize size;
};

// Index buffer
struct GVR_VK_Indices {
    VkDeviceMemory memory;
    VkBuffer buffer;
    uint32_t count;
};

class ImageCreateInfo final
    {
    public:
        ImageCreateInfo(VkImageType aImageType, VkFormat aFormat,
            int32_t aWidth, int32_t aHeight, int32_t aDepth,
            VkImageTiling aTiling, VkImageUsageFlags aUsage,
            VkImageLayout aLayout = VK_IMAGE_LAYOUT_UNDEFINED);

        ImageCreateInfo(VkImageType aImageType, VkFormat aFormat,
            int32_t aWidth, int32_t aHeight, int32_t aDepth,
            uint32_t aArraySize, VkImageTiling aTiling, VkImageUsageFlags aUsage,
            VkImageLayout aLayout = VK_IMAGE_LAYOUT_UNDEFINED);

        ImageCreateInfo(VkImageType aImageType, VkFormat aFormat,
            int32_t aWidth, int32_t aHeight, int32_t aDepth, uint32_t aMipLevels,
            uint32_t aArraySize, VkImageTiling aTiling, VkImageUsageFlags aUsage,
            VkSampleCountFlagBits aSamples = VK_SAMPLE_COUNT_1_BIT,
            VkImageLayout aLayout = VK_IMAGE_LAYOUT_UNDEFINED);


        operator const VkImageCreateInfo*() const
        {
            return &mInfo;
        }
    private:
        VkImageCreateInfo mInfo;
    };


class ImageViewCreateInfo final
    {
    public:
        ImageViewCreateInfo(VkImage aImage, VkImageViewType aType, VkFormat aFormat, VkImageAspectFlags aAspectFlags);
        ImageViewCreateInfo(VkImage aImage, VkImageViewType aType, VkFormat aFormat, uint32_t aArraySize, VkImageAspectFlags aAspectFlags);
        ImageViewCreateInfo(VkImage aImage, VkImageViewType aType, VkFormat aFormat, uint32_t aMipLevels, uint32_t aArraySize, VkImageAspectFlags aAspectFlags);

        operator const VkImageViewCreateInfo*() const
        {
            return &mInfo;
        }
    private:
        VkImageViewCreateInfo mInfo;
    };

class CmdPoolCreateInfo final
    {
        VkCommandPoolCreateInfo mInfo;
    public:
        explicit CmdPoolCreateInfo(VkCommandPoolCreateFlags aFlags = 0, uint32_t aFamilyIndex = 0);

        operator const VkCommandPoolCreateInfo*() const
        {
            return &mInfo;
        }
    };
class DescriptorWrite final
{
    VkWriteDescriptorSet write;
    public:
        explicit DescriptorWrite(VkStructureType type, int& index, VkDescriptorSet& descriptor, int descriptorCount, VkDescriptorType& descriptorType, VkDescriptorBufferInfo& info,
                                    VkDescriptorImageInfo* descriptorImageInfo =0);

           operator const VkWriteDescriptorSet*() const
           {
               return &write;
           }

};

class  DescriptorLayout final
{
    VkDescriptorSetLayoutBinding uniformAndSamplerBinding;
    public:
        explicit DescriptorLayout(int binding, int descriptorCount, VkDescriptorType& descriptorType, int stageFlags, int immulableSamplers);
        operator const VkDescriptorSetLayoutBinding*()const
        {
            return &uniformAndSamplerBinding;
        }
};
class CmdBufferCreateInfo final
    {
        VkCommandBufferAllocateInfo mInfo;
    public:
        explicit CmdBufferCreateInfo(VkCommandBufferLevel aLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY, VkCommandPool aCmdPool = VK_NULL_HANDLE);

        operator const VkCommandBufferAllocateInfo*() const
        {
            return &mInfo;
        }
    };

class BufferCreateInfo final
    {
    public:
        BufferCreateInfo(VkDeviceSize aSize, VkBufferUsageFlags aUsageFlags,
            VkBufferCreateFlags aCreateFlags = 0);

        operator const VkBufferCreateInfo*() const
        {
            return &mInfo;
        }
    private:
        VkBufferCreateInfo mInfo;
    };

class ShaderModuleCreateInfo final
    {
        VkShaderModuleCreateInfo mCreateInfo;
    public:
        ShaderModuleCreateInfo(const uint32_t* aSource, size_t aSize, VkShaderModuleCreateFlags aFlags = 0);

        operator const VkShaderModuleCreateInfo*() const
        {
            return &mCreateInfo;
        }
    };

class SemaphoreCreateInfo final
    {
    public:
        SemaphoreCreateInfo(VkSemaphoreCreateFlags aFlags = 0);

        operator const VkSemaphoreCreateInfo*() const
        {
            return &mInfo;
        }
    private:
        VkSemaphoreCreateInfo mInfo;
    };

class FenceCreateInfo final
    {
    public:
        explicit FenceCreateInfo(VkFenceCreateFlags aFlags = 0);

        operator const VkFenceCreateInfo*() const
        {
            return &mInfo;
        }
    private:
        VkFenceCreateInfo mInfo;
    };
}
#endif //FRAMEWORK_VULKANINFOWRAPPER_H
