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

//struct DirLight {
//    vec4 direction;
//    vec4 ambient;
//    vec4 diffuse;
//    vec4 specular;
//};
uniform vec4 dirLight[4];
vec3 CalcDirLight(vec4 light[4], vec3 normal, vec3 viewDir, vec2 v_texcoord0)
{
    vec3 lightDir = normalize(-light[0].xyz);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess.x);
    // 合并结果
    vec3 ambient  = light[1].xyz  * vec3(texture2D(s_diffuse, v_texcoord0));
    vec3 diffuse  = light[2].xyz  * diff * vec3(texture2D(s_diffuse, v_texcoord0));
    vec3 specular = light[3].xyz * spec * vec3(texture2D(s_specular, v_texcoord0));
    return (ambient + diffuse + specular);
}

//struct PointLight {
//    vec4 position;
//    vec4 constant_para;
//    vec4 linear_para;
//    vec4 quadratic_para;
//    vec4 ambient;
//    vec4 diffuse;
//    vec4 specular;
//};
uniform vec4 pointLights0[7];
uniform vec4 pointLights1[7];
uniform vec4 pointLights2[7];
uniform vec4 pointLights3[7];
vec3 CalcPointLight(vec4 light[7], vec3 normal, vec3 fragPos, vec3 viewDir, vec2 v_texcoord0)
{
    vec3 lightDir = normalize(light[0].xyz - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess.x);
    // 衰减
    float distance    = length(light[0].xyz - fragPos);
    float attenuation = 1.0 / (light[1].x + light[2].x * distance +
                 light[3].x * (distance * distance));
    // 合并结果
    vec3 ambient  = light[4].xyz  * vec3(texture2D(s_diffuse, v_texcoord0));
    vec3 diffuse  = light[5].xyz  * diff * vec3(texture2D(s_diffuse, v_texcoord0));
    vec3 specular = light[6].xyz * spec * vec3(texture2D(s_specular, v_texcoord0));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

//struct SpotLight {
//    vec4 position;
//    vec4 direction;
//    vec4 cutOff;
//    vec4 outerCutOff;
//
//    vec4 constant_para;
//    vec4 linear_para;
//    vec4 quadratic_para;
//
//    vec4 ambient;
//    vec4 diffuse;
//    vec4 specular;
//};
uniform vec4 spotLight[10];
vec3 CalcSpotLight(vec4 light[10], vec3 normal, vec3 fragPos, vec3 viewDir, vec2 v_texcoord0)
{
    vec3 lightDir = normalize(light[0].xyz - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess.x);
    // attenuation
    float distance = length(light[0].xyz - fragPos);
    float attenuation = 1.0 / (light[4].x + light[5].x * distance + light[6].x * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light[1].xyz));
    float epsilon = light[2].x - light[3].x;
    float intensity = clamp((theta - light[3].x) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light[7].xyz * vec3(texture2D(s_diffuse, v_texcoord0));
    vec3 diffuse = light[8].xyz * diff * vec3(texture2D(s_diffuse, v_texcoord0));
    vec3 specular = light[9].xyz * spec * vec3(texture2D(s_specular, v_texcoord0));
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
    vec3 result = CalcDirLight(dirLight, norm, viewDir, v_texcoord0);
    // phase 2: point lights
    result += CalcPointLight(pointLights0, norm, v_FragPos, viewDir, v_texcoord0);
    result += CalcPointLight(pointLights1, norm, v_FragPos, viewDir, v_texcoord0);
    result += CalcPointLight(pointLights2, norm, v_FragPos, viewDir, v_texcoord0);
    result += CalcPointLight(pointLights3, norm, v_FragPos, viewDir, v_texcoord0);
    // phase 3: spot light
    result += CalcSpotLight(spotLight, norm, v_FragPos, viewDir, v_texcoord0);
    
    gl_FragColor = vec4(result, 1.0);
}
