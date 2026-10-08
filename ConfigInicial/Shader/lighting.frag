#version 330 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

uniform sampler2D texture_diffuse1;
uniform vec3 materialColor;
uniform int hasTexture;

void main()
{
    // Ambient
    vec3 ambient = light.ambient * material.diffuse;
    
    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);
    
    // Iluminación
    vec3 result = ambient + diffuse + specular;

    // Textura o color del material
    if (hasTexture == 1)
    {
        vec4 texColor = texture(texture_diffuse1, TexCoords);

        if (texColor.a < 0.1)
            discard;

        color = vec4(result, 1.0) * texColor;
    }
    else
    {
        color = vec4(result * materialColor, 1.0);
    }
}