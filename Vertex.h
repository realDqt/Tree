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
            std::size_t hashPos = std::hash<float>()(vertex.pos.x) ^
                                  (std::hash<float>()(vertex.pos.y) << 1) ^
                                  (std::hash<float>()(vertex.pos.z) << 2);

            std::size_t hashColor= std::hash<float>()(vertex.color.x) ^
                                     (std::hash<float>()(vertex.color.y) << 1) ^
                                     (std::hash<float>()(vertex.color.z) << 2);
            return hashPos ^ (hashColor << 1);
        }
    };
}


// Marry
struct VertexMarry {
    glm::vec3 pos;
    glm::vec2 texCoord;
    glm::vec3 normal;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(VertexMarry);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(VertexMarry, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(VertexMarry, texCoord);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(VertexMarry, normal);

        return attributeDescriptions;
    }

    bool operator==(const VertexMarry& other) const {
        return pos == other.pos && texCoord == other.texCoord && normal == other.normal;
    }
};

namespace std {
    template<> struct hash<VertexMarry> {
        size_t operator()(VertexMarry const& vertex) const {
            // 使用 std::hash 组合 pos、texCoord 和 normal 的哈希值
            std::size_t hashPos = std::hash<float>()(vertex.pos.x) ^
                                  (std::hash<float>()(vertex.pos.y) << 1) ^
                                  (std::hash<float>()(vertex.pos.z) << 2);

            std::size_t hashTexCoord = std::hash<float>()(vertex.texCoord.x) ^
                                       (std::hash<float>()(vertex.texCoord.y) << 1);

            std::size_t hashNormal = std::hash<float>()(vertex.normal.x) ^
                                     (std::hash<float>()(vertex.normal.y) << 1) ^
                                     (std::hash<float>()(vertex.normal.z) << 2);

            // 组合所有哈希值
            return hashPos ^ (hashTexCoord << 1) ^ (hashNormal << 2);
        }
    };
}


// Light
struct VertexLight {
    glm::vec3 pos;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(VertexLight);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 1> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(VertexLight, pos);

        return attributeDescriptions;
    }

    bool operator==(const VertexLight& other) const {
        return pos == other.pos;
    }
};

// Box4IBL
struct VertexBoxIBL {
    glm::vec3 pos;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(VertexBoxIBL);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 1> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(VertexBoxIBL, pos);

        return attributeDescriptions;
    }

    bool operator==(const VertexBox& other) const {
        return pos == other.pos;
    }
};

namespace std {
    template<> struct hash<VertexBoxIBL> {
        size_t operator()(VertexBoxIBL const& vertex) const {
            std::size_t hashPos = std::hash<float>()(vertex.pos.x) ^
                                  (std::hash<float>()(vertex.pos.y) << 1) ^
                                  (std::hash<float>()(vertex.pos.z) << 2);
            return hashPos;
        }
    };
}


// sphere
struct VertexSphere {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texCoord;


    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(VertexSphere);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(VertexSphere, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(VertexSphere, normal);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(VertexSphere, texCoord);


        return attributeDescriptions;
    }

    bool operator==(const VertexSphere& other) const {
        return pos == other.pos && texCoord == other.texCoord && normal == other.normal;
    }
};

namespace std {
    template<> struct hash<VertexSphere> {
        size_t operator()(VertexSphere const& vertex) const {
            // 使用 std::hash 组合 pos、texCoord 和 normal 的哈希值
            std::size_t hashPos = std::hash<float>()(vertex.pos.x) ^
                                  (std::hash<float>()(vertex.pos.y) << 1) ^
                                  (std::hash<float>()(vertex.pos.z) << 2);

            std::size_t hashTexCoord = std::hash<float>()(vertex.texCoord.x) ^
                                       (std::hash<float>()(vertex.texCoord.y) << 1);

            std::size_t hashNormal = std::hash<float>()(vertex.normal.x) ^
                                     (std::hash<float>()(vertex.normal.y) << 1) ^
                                     (std::hash<float>()(vertex.normal.z) << 2);

            // 组合所有哈希值
            return hashPos ^ (hashNormal << 1) ^ (hashTexCoord << 2);
        }
    };
}


#endif //VKRENDERINGENGINE_VERTEX_H
