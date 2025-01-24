#version 450

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec3 worldNormal;
layout(location = 3) in vec4 lightSpaceCoord;


layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;

layout(binding = 2, std140) uniform DirectionalLight{
    vec3 lightDir;
    vec3 lightIntensity;
} light;

layout(push_constant, std140)uniform PushConstants{
    vec3 cameraPos;
    bool isFloor;
} constants;

layout(binding = 3) uniform sampler2D smSampler;

// Shadow map related variables
#define NUM_SAMPLES 100
#define BLOCKER_SEARCH_NUM_SAMPLES NUM_SAMPLES
#define PCF_NUM_SAMPLES NUM_SAMPLES
#define NUM_RINGS 10

#define EPS 1e-2
#define PI 3.141592653589793
#define PI2 6.283185307179586


float unpack(vec4 rgbaDepth) {
    const vec4 bitShift = vec4(1.0, 1.0/256.0, 1.0/(256.0*256.0), 1.0/(256.0*256.0*256.0));
    return dot(rgbaDepth, bitShift);
}

float rand_1to1(float x ) {
    // -1 -1
    return fract(sin(x)*10000.0);
}

float rand_2to1(vec2 uv ) {
    // 0 - 1
    const float a = 12.9898, b = 78.233, c = 43758.5453;
    float dt = dot( uv.xy, vec2( a,b ) ), sn = mod( dt, PI );
    return fract(sin(sn) * c);
}

vec2 poissonDisk[NUM_SAMPLES];

void poissonDiskSamples( const in vec2 randomSeed ) {

    float ANGLE_STEP = PI2 * float( NUM_RINGS ) / float( NUM_SAMPLES );
    float INV_NUM_SAMPLES = 1.0 / float( NUM_SAMPLES );

    float angle = rand_2to1( randomSeed ) * PI2;
    float radius = INV_NUM_SAMPLES;
    float radiusStep = radius;

    for( int i = 0; i < NUM_SAMPLES; i ++ ) {
        poissonDisk[i] = vec2( cos( angle ), sin( angle ) ) * pow( radius, 0.75 );
        radius += radiusStep;
        angle += ANGLE_STEP;
    }
}

float getBias(float ctrl)
{
    float m = 10.0 / 2048.0 / 2.0;
    float bias = max(m, m * (1.0 - dot(worldNormal, light.lightDir))) * ctrl;
    return bias;
}

void uniformDiskSamples( const in vec2 randomSeed ) {

    float randNum = rand_2to1(randomSeed);
    float sampleX = rand_1to1( randNum ) ;
    float sampleY = rand_1to1( sampleX ) ;

    float angle = sampleX * PI2;
    float radius = sqrt(sampleY);

    for( int i = 0; i < NUM_SAMPLES; i ++ ) {
        poissonDisk[i] = vec2( radius * cos(angle) , radius * sin(angle)  );

        sampleX = rand_1to1( sampleY ) ;
        sampleY = rand_1to1( sampleX ) ;

        angle = sampleX * PI2;
        radius = sqrt(sampleY);
    }
}

float findBlocker(vec2 uv, float zReceiver ) {

    int blockerNum = 0;
    float blockDepth = 0;
    const int shadowmapSize = 2048;
    const float stride = 50;

    poissonDiskSamples(uv);

    for(int i = 0; i < NUM_SAMPLES; ++i){
        vec4 shadowColor = texture(smSampler, uv + poissonDisk[i] * stride / float(shadowmapSize));
        float shadowDepth = unpack(shadowColor);
        if(zReceiver > shadowDepth + EPS){
            blockerNum++;
            blockDepth += shadowDepth;
        }

    }
    if(blockerNum == 0){
        return 1.0;
    }
    return blockDepth / float(blockerNum);
}

float PCF(vec4 coords) {

    float dBlocker = findBlocker(coords.xy, coords.z);
    float wLight = 1.0;
    float dReceiver = coords.z;
    float wPenmbra = wLight * (dReceiver - dBlocker) / dBlocker;

    const float stride = 5.0;
    const int shadowmapSize = 2048;
    float visbility = 0.0;
    float curDepth = coords.z;

    for(int i = 0; i < NUM_SAMPLES; ++i){
        vec4 shadowColor = texture(smSampler, coords.xy + poissonDisk[i] * stride / float(shadowmapSize) * wPenmbra);
        float shadowDepth = unpack(shadowColor);
        float res = curDepth <= shadowDepth + getBias(1.4) ? 1.0 : 0.0;
        visbility += res;
    }

    return visbility / float(NUM_SAMPLES);
}

float PCSS(vec4 coords){

    // STEP 1: avgblocker depth

    // STEP 2: penumbra size

    // STEP 3: filtering

    return PCF(coords);

}
vec3 albedo = vec3(0.0);
vec3 uKs = vec3(0.0);

vec3 PhongColor()
{
    //vec3 ambient = 0.05 * albedo;

    float diff = max(0, dot(light.lightDir, worldNormal));
    vec3 diffuse = diff * albedo * light.lightIntensity;

    vec3 viewDir = constants.cameraPos - fragPos;
    vec3 halfVec = normalize(viewDir + light.lightDir);
    float spec = pow(max(0, dot(halfVec, worldNormal)), 32.f);
    vec3 specular = uKs * spec * light.lightIntensity;

    return diffuse + specular;
}


float GetVisbility()
{
    // calculate visbility
    vec3 NDC = lightSpaceCoord.xyz / lightSpaceCoord.w;
    NDC.xy = (NDC.xy + 1.0f) * .5f;
    return PCSS(vec4(NDC, 1.0));
}

void main()
{
    if(constants.isFloor){
        albedo = vec3(0.8);
    }else{
        albedo = texture(texSampler, fragTexCoord).rgb;
    }


    /*
    vec3 phongColor = PhongColor();

    float visbility = GetVisbility();

    outColor = vec4(pow(visbility * phongColor + 0.05 * albedo, vec3(1.0/2.2)), 1.0);
    */


    vec3 phongColor = PhongColor() + 0.05 * albedo;
    float visbility = GetVisbility();
    outColor = vec4(pow(visbility * phongColor, vec3(1.0/2.2)), 1.0);

}