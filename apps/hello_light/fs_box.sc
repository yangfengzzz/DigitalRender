$input v_texcoord0
$input v_normal
$input v_FragPos

/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "../common/common.sh"

uniform vec4 viewPos;

SAMPLER2D(s_diffuse,  0);
SAMPLER2D(s_specular, 1);
uniform vec4 material[2];
#define material_ambient  material[0]
#define material_shininess   material[1]

uniform vec4 dirLight[4];
#define dirLight_direction dirLight[0]
#define dirLight_ambient dirLight[1]
#define dirLight_diffuse dirLight[2]
#define dirLight_specular dirLight[3]
vec3 CalcDirLight(vec3 normal, vec3 viewDir, vec2 v_texcoord0)
{
    vec3 lightDir = normalize(-dirLight_direction.xyz);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess.x);
    // 合并结果
    vec3 ambient  = dirLight_ambient.xyz  * vec3(texture2D(s_diffuse, v_texcoord0));
    vec3 diffuse  = dirLight_diffuse.xyz  * diff * vec3(texture2D(s_diffuse, v_texcoord0));
    vec3 specular = dirLight_specular.xyz * spec * vec3(texture2D(s_specular, v_texcoord0));
    return (ambient + diffuse + specular);
}

#define NR_POINT_LIGHTS 4
uniform vec4 pointLights_position[NR_POINT_LIGHTS];
uniform vec4 pointLights_paras;
uniform vec4 pointLights_ambient;
uniform vec4 pointLights_diffuse;
uniform vec4 pointLights_specular;
#define pointLights_constant pointLights_paras.x
#define pointLights_linear pointLights_paras.y
#define pointLights_quadratic pointLights_paras.z
vec3 CalcPointLight(int _idx, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 v_texcoord0)
{
    vec3 lightDir = normalize(pointLights_position[_idx].xyz - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess.x);
    // 衰减
    float distance    = length(pointLights_position[_idx].xyz - fragPos);
    float attenuation = 1.0 / (pointLights_constant + pointLights_linear * distance +
                 pointLights_quadratic * (distance * distance));
    // 合并结果
    vec3 ambient  = pointLights_ambient.xyz  * vec3(texture2D(s_diffuse, v_texcoord0));
    vec3 diffuse  = pointLights_diffuse.xyz  * diff * vec3(texture2D(s_diffuse, v_texcoord0));
    vec3 specular = pointLights_specular.xyz * spec * vec3(texture2D(s_specular, v_texcoord0));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

uniform vec4 spotLight[7];
#define spotLight_position spotLight[0]
#define spotLight_direction spotLight[1]
#define spotLight_cutOff spotLight[2].x
#define spotLight_outerCutOff spotLight[2].y
#define spotLight_constant spotLight[3].x
#define spotLight_linear spotLight[3].y
#define spotLight_quadratic spotLight[3].z
#define spotLight_ambient spotLight[4]
#define spotLight_diffuse spotLight[5]
#define spotLight_specular spotLight[6]
vec3 CalcSpotLight(vec3 normal, vec3 fragPos, vec3 viewDir, vec2 v_texcoord0)
{
    vec3 lightDir = normalize(spotLight_position.xyz - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess.x);
    // attenuation
    float distance = length(spotLight_position.xyz - fragPos);
    float attenuation = 1.0 / (spotLight_constant + spotLight_linear * distance + spotLight_quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-spotLight_direction.xyz));
    float epsilon = spotLight_cutOff - spotLight_outerCutOff;
    float intensity = clamp((theta - spotLight_outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = spotLight_ambient.xyz * vec3(texture2D(s_diffuse, v_texcoord0));
    vec3 diffuse = spotLight_diffuse.xyz * diff * vec3(texture2D(s_diffuse, v_texcoord0));
    vec3 specular = spotLight_specular.xyz * spec * vec3(texture2D(s_specular, v_texcoord0));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

void main()
{
    // properties
    vec3 norm = normalize(v_normal);
    vec3 viewDir = normalize(vec3(viewPos) - v_FragPos);
    
    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    // phase 1: directional lighting
    vec3 result = CalcDirLight(norm, viewDir, v_texcoord0);
    // phase 2: point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(i, norm, v_FragPos, viewDir, v_texcoord0);
    // phase 3: spot light
    result += CalcSpotLight(norm, v_FragPos, viewDir, v_texcoord0);
    
    gl_FragColor = vec4(result, 1.0);
}
