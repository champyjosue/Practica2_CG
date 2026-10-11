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

uniform Light sun;
uniform Light moon;

uniform sampler2D texture_diffuse1;
uniform vec3 materialColor;
uniform int hasTexture;

uniform bool sunEnabled;
uniform bool moonEnabled;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // =========================
    // SOL
    // =========================
    vec3 sunLightDir = normalize(sun.position - FragPos);

    float sunDiff = max(dot(norm, sunLightDir), 0.0);
    vec3 sunDiffuse = sun.diffuse * sunDiff * material.diffuse;

    vec3 sunReflectDir = reflect(-sunLightDir, norm);
    float sunSpec = pow(
        max(dot(viewDir, sunReflectDir), 0.0),
        material.shininess
    );

    vec3 sunSpecular =
        sun.specular * sunSpec * material.specular;


    // =========================
    // LUNA
    // =========================
    vec3 moonLightDir = normalize(moon.position - FragPos);

    float moonDiff = max(dot(norm, moonLightDir), 0.0);
    vec3 moonDiffuse = moon.diffuse * moonDiff * material.diffuse;

    vec3 moonReflectDir = reflect(-moonLightDir, norm);
    float moonSpec = pow(
        max(dot(viewDir, moonReflectDir), 0.0),
        material.shininess
    );

    vec3 moonSpecular =
        moon.specular * moonSpec * material.specular;


    // =========================
    // RESULTADO SEGÚN LA LUZ ACTIVA
    // =========================
    vec3 result = vec3(0.0);

    if (sunEnabled)
    {
        result += sun.ambient * material.diffuse;
        result += sunDiffuse + sunSpecular;
    }

    if (moonEnabled)
    {
        result += moon.ambient * material.diffuse;
        result += moonDiffuse + moonSpecular;
    }


    // =========================
    // TEXTURA O COLOR
    // =========================
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