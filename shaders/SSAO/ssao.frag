#version 450
layout(location = 0) in vec2 TexCoords;

layout(location = 0) out float Occlusion;

layout(binding = 0) uniform sampler2D gViewPositionSampler;
layout(binding = 1) uniform sampler2D gDepthSampler;
layout(binding = 2) uniform sampler2D gViewNormalSampler;
layout(binding = 3) uniform sampler2D texNoiseSampler;

layout(binding = 4, std140) uniform UniformBufferObject{
    mat4 projection;
    vec3 samples[64];
} ubo;

// parameters (you'd probably want to use them as uniforms to more easily tweak the effect)
int kernelSize = 64;
float radius = 1.0;

// tile noise texture over screen based on screen dimensions divided by noise size
const vec2 noiseScale = vec2(800.0f/4.0f, 600.0f/4.0f);


void main()
{
    // Get input for SSAO algorithm
    vec3 fragPos = texture(gViewPositionSampler, TexCoords).xyz;
    vec3 normal = texture(gViewNormalSampler, TexCoords).rgb;
    vec3 randomVec = texture(texNoiseSampler, TexCoords * noiseScale).xyz;
    // Create TBN change-of-basis matrix: from tangent-space to view-space
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    // Iterate over the sample kernel and calculate occlusion factor
    float occlusion = 0.0;
    float bias = 0.05; // 使用一个小的偏移量来防止“表面摩尔纹”或自遮挡

    for(int i = 0; i < kernelSize; ++i)
    {
        // 获取视图空间下的采样点位置
        vec3 samplePos = TBN * ubo.samples[i];
        samplePos = fragPos + samplePos * radius;

        // 将采样点投影到屏幕空间以获取UV坐标
        vec4 offset = vec4(samplePos, 1.0);
        offset = ubo.projection * offset; // 视图空间 -> 裁剪空间
        offset.xyz /= offset.w;           // 透视除法 -> NDC
        offset.xy = offset.xy * 0.5 + 0.5;  // NDC -> UV [0,1]

        // --- BUG FIX ---
        // 1. 从gDepthSampler获取该UV坐标处已存在的几何体的线性深度
        float occluderLinearDepth = texture(gDepthSampler, offset.xy).r;

        // 2. 获取当前采样点的线性深度
        //    samplePos.z 是视图空间的Z坐标(负值)。clippos.w = -samplePos.z
        float sampleLinearDepth = -samplePos.z;

        // 3. 在线性视图空间中进行比较
        //    当采样点比它前方的几何体“更远”时，才算被遮挡。
        //    “更远”意味着线性深度值更大。
        //    添加一个 bias 防止自遮挡。
        if (sampleLinearDepth > occluderLinearDepth + bias)
        {
            // 4. (可选但推荐) 添加一个范围检查，让遮蔽效果随距离衰减
            float rangeCheck = smoothstep(0.0, 1.0, radius / abs(abs(fragPos.z) - occluderLinearDepth));
            occlusion += 1.0 * rangeCheck;
        }
    }
    occlusion = 1.0 - (occlusion / kernelSize);

    Occlusion = occlusion;
}