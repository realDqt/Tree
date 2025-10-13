#version 450
layout(location = 0) in vec2 TexCoords;

layout(location = 0) out float FragColor;

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
    for(int i = 0; i < kernelSize; ++i)
    {
        // get sample position
        vec3 samplePos = TBN * ubo.samples[i]; // From tangent to view-space
        samplePos = fragPos + samplePos * radius;

        // project sample position (to sample texture) (to get position on screen/texture)
        vec4 offset = vec4(samplePos, 1.0);
        offset = ubo.projection * offset; // from view to clip-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0

        // get sample depth
        float sampleDepth = -texture(gDepthSampler, offset.xy).r; // Get depth value of kernel sample

        // range check & accumulate
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth ));
        occlusion += (sampleDepth >= samplePos.z ? 1.0 : 0.0) * rangeCheck;
    }
    occlusion = 1.0 - (occlusion / kernelSize);

    FragColor = occlusion;
}