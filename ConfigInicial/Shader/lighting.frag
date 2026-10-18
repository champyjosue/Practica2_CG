#version 330 core

#define NUMBER_OF_POINT_LIGHTS 5

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

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

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

// 0 = objeto OPACO (se ignora la transparencia)
// 1 = objeto TRANSPARENTE (zonas oscuras de la textura = transparentes)
uniform int transparency;

// Los manda Mesh.h: color del .mtl y si el modelo tiene textura
uniform vec3 materialColor;
uniform int hasTexture;

// Valores base del fragmento (se calculan una vez en main)
vec3 baseDiffuse;
vec3 baseSpecular;
float maskAlpha;

// Function prototypes
vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir );
vec3 CalcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir );
vec3 CalcSpotLight( SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir );

void main( )
{
    // Color base: textura si el modelo tiene, si no el color del .mtl
    if ( hasTexture == 1 )
    {
        vec4 t = texture( material.diffuse, TexCoords );
        baseDiffuse = t.rgb;
        maskAlpha = t.r;   // la transparencia sale del canal rojo de la textura
        baseSpecular = texture( material.specular, TexCoords ).rgb;
    }
    else
    {
        baseDiffuse = materialColor;
        maskAlpha = 1.0;
        baseSpecular = vec3( 0.2 );
    }

    // Properties
    vec3 norm = normalize( Normal );
    vec3 viewDir = normalize( viewPos - FragPos );

    // Directional lighting
    vec3 result = CalcDirLight( dirLight, norm, viewDir );

    // Point lights
    for ( int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++ )
    {
        result += CalcPointLight( pointLights[i], norm, FragPos, viewDir );
    }

    // Spot light
    result += CalcSpotLight( spotLight, norm, FragPos, viewDir );

    // Alfa final: opaco por defecto, transparente solo si transparency == 1
    float finalAlpha = 1.0;
    if ( transparency == 1 )
    {
        finalAlpha = maskAlpha;
        if ( finalAlpha < 0.1 )
            discard;
    }

    color = vec4( result, finalAlpha );
}

// Calculates the color when using a directional light.
vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir )
{
    vec3 lightDir = normalize( -light.direction );

    float diff = max( dot( normal, lightDir ), 0.0 );

    vec3 reflectDir = reflect( -lightDir, normal );
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess );

    vec3 ambient = light.ambient * baseDiffuse;
    vec3 diffuse = light.diffuse * diff * baseDiffuse;
    vec3 specular = light.specular * spec * baseSpecular;

    return ( ambient + diffuse + specular );
}

// Calculates the color when using a point light.
vec3 CalcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir )
{
    vec3 lightDir = normalize( light.position - fragPos );

    float diff = max( dot( normal, lightDir ), 0.0 );

    vec3 reflectDir = reflect( -lightDir, normal );
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess );

    float distance = length( light.position - fragPos );
    float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ) );

    vec3 ambient = light.ambient * baseDiffuse;
    vec3 diffuse = light.diffuse * diff * baseDiffuse;
    vec3 specular = light.specular * spec * baseSpecular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ( ambient + diffuse + specular );
}

// Calculates the color when using a spot light.
vec3 CalcSpotLight( SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir )
{
    vec3 lightDir = normalize( light.position - fragPos );

    float diff = max( dot( normal, lightDir ), 0.0 );

    vec3 reflectDir = reflect( -lightDir, normal );
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess );

    float distance = length( light.position - fragPos );
    float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ) );

    float theta = dot( lightDir, normalize( -light.direction ) );
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp( ( theta - light.outerCutOff ) / epsilon, 0.0, 1.0 );

    vec3 ambient = light.ambient * baseDiffuse;
    vec3 diffuse = light.diffuse * diff * baseDiffuse;
    vec3 specular = light.specular * spec * baseSpecular;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ( ambient + diffuse + specular );
}
