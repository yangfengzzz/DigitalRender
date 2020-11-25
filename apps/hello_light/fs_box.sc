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

struct DirLight {
    vec4 direction;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};
uniform DirLight dirLight;
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec2 v_texcoord0)
{
    vec3 lightDir = normalize(-light.direction.xyz);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess.x);
    // 合并结果
    vec3 ambient  = light.ambient.xyz  * vec3(texture2D(s_diffuse, v_texcoord0));
    vec3 diffuse  = light.diffuse.xyz  * diff * vec3(texture2D(s_diffuse, v_texcoord0));
    vec3 specular = light.specular.xyz * spec * vec3(texture2D(s_specular, v_texcoord0));
    return (ambient + diffuse + specular);
}

struct PointLight {
    vec4 position;
    vec4 constant_para;
    vec4 linear_para;
    vec4 quadratic_para;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 v_texcoord0)
{
    vec3 lightDir = normalize(light.position.xyz - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess.x);
    // 衰减
    float distance    = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (light.constant_para.x + light.linear_para.x * distance +
                 light.quadratic_para.x * (distance * distance));
    // 合并结果
    vec3 ambient  = light.ambient.xyz  * vec3(texture2D(s_diffuse, v_texcoord0));
    vec3 diffuse  = light.diffuse.xyz  * diff * vec3(texture2D(s_diffuse, v_texcoord0));
    vec3 specular = light.specular.xyz * spec * vec3(texture2D(s_specular, v_texcoord0));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

struct SpotLight {
    vec4 position;
    vec4 direction;
    vec4 cutOff;
    vec4 outerCutOff;
  
    vec4 constant_para;
    vec4 linear_para;
    vec4 quadratic_para;
  
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};
uniform SpotLight spotLight;
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 v_texcoord0)
{
    vec3 lightDir = normalize(light.position.xyz - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess.x);
    // attenuation
    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (light.constant_para.x + light.linear_para.x * distance + light.quadratic_para.x * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction.xyz));
    float epsilon = light.cutOff.x - light.outerCutOff.x;
    float intensity = clamp((theta - light.outerCutOff.x) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient.xyz * vec3(texture2D(s_diffuse, v_texcoord0));
    vec3 diffuse = light.diffuse.xyz * diff * vec3(texture2D(s_diffuse, v_texcoord0));
    vec3 specular = light.specular.xyz * spec * vec3(texture2D(s_specular, v_texcoord0));
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
    //for(int i = 0; i < NR_POINT_LIGHTS; i++)
    //    result += CalcPointLight(pointLights[i], norm, v_FragPos, viewDir, v_texcoord0);
    // phase 3: spot light
    //result += CalcSpotLight(spotLight, norm, v_FragPos, viewDir, v_texcoord0);
    
    gl_FragColor = vec4(result, 1.0);
}
