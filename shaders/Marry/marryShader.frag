#version 450

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec3 worldNormal;


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

void main()
{

    vec3 albedo = vec3(0.0);
    vec3 uKs = vec3(0.0);

    if(constants.isFloor){
        albedo = vec3(0.8);
        uKs = vec3(0.8);
    }else{
        albedo = texture(texSampler, fragTexCoord).rgb;
    }


    vec3 ambient = 0.05 * albedo;

    float diff = max(0, dot(light.lightDir, worldNormal));
    vec3 diffuse = diff * albedo * light.lightIntensity;

    vec3 viewDir = constants.cameraPos - fragPos;
    vec3 halfVec = normalize(viewDir + light.lightDir);
    float spec = pow(max(0, dot(halfVec, worldNormal)), 32.f);
    vec3 specular = uKs * spec * light.lightIntensity;


    outColor = vec4(pow(ambient + diffuse + specular, vec3(1.0/2.2)), 1.0);
}