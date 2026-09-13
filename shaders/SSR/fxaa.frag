#version 450

layout(location = 0) in vec2 texCoords;
layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform sampler2D inputSampler;

const float EDGE_THRESHOLD_MIN = 0.0312;
const float EDGE_THRESHOLD_MAX = 0.125;
const float REDUCE_MIN = 0.0078125;
const float REDUCE_MUL = 0.125;
const float SPAN_MAX = 8.0;

vec2 ClampUv(vec2 uv, vec2 texelSize) {
    return clamp(uv, texelSize * 0.5, vec2(1.0) - texelSize * 0.5);
}

vec3 SampleColor(vec2 uv, vec2 texelSize) {
    return textureLod(inputSampler, ClampUv(uv, texelSize), 0.0).rgb;
}

float Luma(vec3 color) {
    return dot(color, vec3(0.299, 0.587, 0.114));
}

void main() {
    vec2 texelSize = 1.0 / vec2(textureSize(inputSampler, 0));
    vec3 rgbCenter = SampleColor(texCoords, texelSize);
    vec3 rgbNorthWest = SampleColor(texCoords + vec2(-1.0, -1.0) * texelSize, texelSize);
    vec3 rgbNorthEast = SampleColor(texCoords + vec2(1.0, -1.0) * texelSize, texelSize);
    vec3 rgbSouthWest = SampleColor(texCoords + vec2(-1.0, 1.0) * texelSize, texelSize);
    vec3 rgbSouthEast = SampleColor(texCoords + vec2(1.0, 1.0) * texelSize, texelSize);

    float lumaCenter = Luma(rgbCenter);
    float lumaNorthWest = Luma(rgbNorthWest);
    float lumaNorthEast = Luma(rgbNorthEast);
    float lumaSouthWest = Luma(rgbSouthWest);
    float lumaSouthEast = Luma(rgbSouthEast);
    float lumaMin = min(lumaCenter, min(min(lumaNorthWest, lumaNorthEast), min(lumaSouthWest, lumaSouthEast)));
    float lumaMax = max(lumaCenter, max(max(lumaNorthWest, lumaNorthEast), max(lumaSouthWest, lumaSouthEast)));
    float lumaRange = lumaMax - lumaMin;

    if (lumaRange < max(EDGE_THRESHOLD_MIN, lumaMax * EDGE_THRESHOLD_MAX)) {
        outColor = vec4(rgbCenter, 1.0);
        return;
    }

    vec2 direction;
    direction.x = -((lumaNorthWest + lumaNorthEast) - (lumaSouthWest + lumaSouthEast));
    direction.y = (lumaNorthWest + lumaSouthWest) - (lumaNorthEast + lumaSouthEast);

    float directionReduce = max((lumaNorthWest + lumaNorthEast + lumaSouthWest + lumaSouthEast) * (0.25 * REDUCE_MUL), REDUCE_MIN);
    float inverseDirectionMin = 1.0 / (min(abs(direction.x), abs(direction.y)) + directionReduce);
    direction = clamp(direction * inverseDirectionMin, vec2(-SPAN_MAX), vec2(SPAN_MAX)) * texelSize;

    vec3 rgbA = 0.5 * (
            SampleColor(texCoords + direction * (-1.0 / 6.0), texelSize) +
            SampleColor(texCoords + direction * (1.0 / 6.0), texelSize)
    );
    vec3 rgbB = rgbA * 0.5 + 0.25 * (
            SampleColor(texCoords + direction * -0.5, texelSize) +
            SampleColor(texCoords + direction * 0.5, texelSize)
    );
    float lumaB = Luma(rgbB);
    vec3 result = lumaB < lumaMin || lumaB > lumaMax ? rgbA : rgbB;
    outColor = vec4(result, 1.0);
}
