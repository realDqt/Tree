//
// Created by 22473 on 2025-01-18.
//

#ifndef VKRENDERINGENGINE_VERTEX_H
#define VKRENDERINGENGINE_VERTEX_H
#include "utils.h"

// Box
struct VertexBox {
    glm::vec3 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(VertexBox);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(VertexBox, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(VertexBox, color);

        return attributeDescriptions;
    }

    bool operator==(const VertexBox& other) const {
        return pos == other.pos && color == other.color;
    }
};

namespace std {
    template<> struct hash<VertexBox> {
        size_t operator()(VertexBox const& vertex) const {
            //return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
            return 0;
        }
    };
}


// Marry
struct VertexMarry {
    glm::vec3 pos;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(VertexMarry);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(VertexMarry, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(VertexMarry, texCoord);

        return attributeDescriptions;
    }

    bool operator==(const VertexMarry& other) const {
        return pos == other.pos && texCoord == other.texCoord;
    }
};

namespace std {
    template<> struct hash<VertexMarry> {
        size_t operator()(VertexMarry const& vertex) const {
            // 使用 std::hash 结合 glm::vec3 和 glm::vec2 的哈希值
            std::size_t h1 = std::hash<float>()(vertex.pos.x) ^ (std::hash<float>()(vertex.pos.y) << 1) ^ (std::hash<float>()(vertex.pos.z) << 2);
            std::size_t h2 = std::hash<float>()(vertex.texCoord.x) ^ (std::hash<float>()(vertex.texCoord.y) << 1);

            // 合并两个哈希值
            return h1 ^ (h2 << 1);
        }
    };
}

#endif //VKRENDERINGENGINE_VERTEX_H
