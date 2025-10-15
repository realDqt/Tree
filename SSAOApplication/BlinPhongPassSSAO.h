//
// Created by Administrator on 2025/10/15.
//

#ifndef VKRENDERINGENGINE_BLINPHONGPASSSSAO_H
#define VKRENDERINGENGINE_BLINPHONGPASSSSAO_H
#include "SSAOutils.h"

class BlinPhongPassSSAO : public PresentPass{
public:
    struct alignas(16) UniformBufferObject{
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
        glm::mat4 modelInvTrans;
    };

    struct alignas(16) UniformBufferObject2{
        glm::vec3 cameraPos;
        alignas(16) glm::vec3 lightDir;
        alignas(16) glm::vec3 lightRadiance;
    };



    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    std::vector<VkBuffer> uniformBuffers2;
    std::vector<VkDeviceMemory> uniformBuffersMemory2;
    std::vector<void*> uniformBuffersMapped2;


    std::optional<glm::mat4> model;

    void init() override;

    void createRenderPass() override;
    void createFramebuffers() override;
    void createGraphicsPipeline() override;

    void createUniformBuffers();

    void createDescriptorSetLayout() override;
    void createDescriptorPool() override;
    void createDescriptorSets() override;

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) override;
    bool IsValid() override;

    void cleanup() override;

    void updateUniformBuffer(uint32_t currentImage);

};
#endif //VKRENDERINGENGINE_BLINPHONGPASSSSAO_H
