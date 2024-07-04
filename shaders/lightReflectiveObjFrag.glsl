#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal; // not particularly normalized
in vec3 Color;
in vec2 TexCoord;

struct Material 
{
    sampler2D diffuse;   // color map
    sampler2D specular;  // specular map - how shiny each area should be
    float     shininess; // intensity of the shininess
};

struct Light 
{
    vec3 position;

    vec3 ambient;  // color
    vec3 diffuse;  // intensity of the diffuse light
    vec3 specular; // intensity of the specular light
};

uniform Material u_Material;
uniform Light u_Light;

uniform vec3 u_ViewPos; // Position of the main camera

vec4 pointLight()
{	
    // Ambient
    vec3 ambient = u_Light.ambient * texture(u_Material.diffuse, TexCoord).rgb;

    // Diffuse 
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(u_Light.position - FragPos); // calculated like that to skip 90-degree counterclockwise rotation for dot product
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = u_Light.diffuse * diff * texture(u_Material.diffuse, TexCoord).rgb;

    // Specular
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal); // learnopengl.com/Lighting/Basic-Lighting
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = u_Light.specular * spec * texture(u_Material.specular, TexCoord).r;

	return vec4(ambient + diffuse + specular, 1.0);
}

vec4 direcLight()
{
    // Ambient
    vec3 ambient = u_Light.ambient * texture(u_Material.diffuse, TexCoord).rgb;

    // Diffuse 
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(u_Light.position - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = u_Light.diffuse * diff * texture(u_Material.diffuse, TexCoord).rgb;

    // Specular
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = u_Light.specular * spec * texture(u_Material.specular, TexCoord).r;

	return vec4(ambient + diffuse + specular, 1.0);
}

void main()
{
    FragColor = direcLight();
}