#version 450

layout(location = 0) in vec2 texCoords;

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform sampler2D indirectSampler;   // temporally accumulated, albedo demodulated
layout(binding = 1) uniform sampler2D momentsSampler;    // (view depth, accumulated sample count)
layout(binding = 2) uniform sampler2D gWorldPositionSampler;
layout(binding = 3) uniform sampler2D gWorldNormalSampler;
layout(binding = 4) uniform sampler2D gDepthSampler;
layout(binding = 5) uniform sampler2D gAlbedoSampler;
layout(binding = 6) uniform sampler2D directLightSampler;

layout(push_constant, std140) uniform PushConstants {
    float maxAccumFrames;
    float phiNormal;   
    float phiPlane;   
} pc;

#define FILTER_RADIUS 2
#define BACKGROUND_DEPTH 99.0

// B3 spline row used by a-trous filters, indexed by abs(offset).
const float KERNEL[FILTER_RADIUS + 1] = float[FILTER_RADIUS + 1](3.0 / 8.0, 1.0 / 4.0, 1.0 / 16.0);

void main() {
    vec2 uv = texCoords;
    float centerDepth = textureLod(gDepthSampler, uv, 0.0).x;
    if (centerDepth >= BACKGROUND_DEPTH) {
        outColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    vec3 centerPos = texture(gWorldPositionSampler, uv).xyz;
    vec3 centerNormal = normalize(texture(gWorldNormalSampler, uv).xyz);

    // Pixels that were just disoccluded carry almost no temporal history, so they are the
    // noisiest ones and get a dilated kernel to reach further for usable neighbours.
    float sampleCount = texture(momentsSampler, uv).y;
    float stride = sampleCount < 0.25 * pc.maxAccumFrames ? 2.0 : 1.0;

    vec2 texelSize = 1.0 / vec2(textureSize(indirectSampler, 0));
    vec3 sum = vec3(0.0);
    float weightSum = 0.0;

    for (int y = -FILTER_RADIUS; y <= FILTER_RADIUS; ++y) {
        for (int x = -FILTER_RADIUS; x <= FILTER_RADIUS; ++x) {
            vec2 sampleUv = uv + vec2(x, y) * stride * texelSize;
            if (any(lessThan(sampleUv, vec2(0.0))) || any(greaterThan(sampleUv, vec2(1.0)))) {
                continue;
            }
            if (textureLod(gDepthSampler, sampleUv, 0.0).x >= BACKGROUND_DEPTH) {
                continue;
            }

            vec3 samplePos = texture(gWorldPositionSampler, sampleUv).xyz;
            vec3 sampleNormal = normalize(texture(gWorldNormalSampler, sampleUv).xyz);
            
            float planeDistance = abs(dot(samplePos - centerPos, centerNormal));
            float weightPlane = exp(-planeDistance / pc.phiPlane);
            float weightNormal = pow(max(dot(centerNormal, sampleNormal), 0.0), pc.phiNormal);
            float weightSpatial = KERNEL[abs(x)] * KERNEL[abs(y)];

            float weight = weightSpatial * weightPlane * weightNormal;
            sum += texture(indirectSampler, sampleUv).rgb * weight;
            weightSum += weight;
        }
    }
    
    vec3 indirect = sum / weightSum;
    
    vec3 albedo = texture(gAlbedoSampler, uv).rgb;
    vec3 directLight = texture(directLightSampler, uv).rgb;
    outColor = vec4(clamp(directLight + albedo * indirect, vec3(0.0), vec3(1.0)), 1.0);
}
