#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal; // not particularly normalized
in vec2 TexCoord;

// All specular light should have only one channel

struct Material 
{
    sampler2D texture_diffuse1;   // color map
    sampler2D texture_specular1;  // specular map - how shiny each area should be
    float     shininess;          // intensity of the shininess
};

struct DirLight 
{
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;
uniform bool u_DirectionLightEnabled;

struct PointLight 
{    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define MAX_POINT_LIGHTS 10 
uniform PointLight pointLights[MAX_POINT_LIGHTS];

struct SpotLight 
{    
    vec3 position;
    vec3 direction;
    float innerCutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define MAX_SPOT_LIGHTS 10  
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform int u_NumPointLights;
uniform int u_NumSpotLights;

uniform Material u_Material;

uniform vec3 u_ViewPos; // Position of the main camera

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir); 
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(u_ViewPos - FragPos);

    vec3 result = vec3(0.0f);

    if(u_DirectionLightEnabled)
    {
        result += CalcDirLight(dirLight, norm, viewDir);
    }

    for(int i = 0; i < u_NumPointLights; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    

    for(int i = 0; i < u_NumSpotLights; i++)
        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);  
    
    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(lightDir, normal), 0.0f);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_Material.shininess);

    vec3 ambient  = light.ambient * vec3(texture(u_Material.texture_diffuse1, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(u_Material.texture_diffuse1, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(u_Material.texture_specular1, TexCoord)).r;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(lightDir, normal), 0.0f);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_Material.shininess);

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));   

    vec3 ambient  = light.ambient * vec3(texture(u_Material.texture_diffuse1, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(u_Material.texture_diffuse1, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(u_Material.texture_specular1, TexCoord)).r;
    ambient  *= attenuation;  diffuse  *= attenuation;  specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(lightDir, normal), 0.0f);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_Material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 

    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = light.ambient * vec3(texture(u_Material.texture_diffuse1, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_Material.texture_diffuse1, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(u_Material.texture_specular1, TexCoord)).r;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}