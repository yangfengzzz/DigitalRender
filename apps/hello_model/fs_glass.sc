$input v_texcoord0
$input v_normal
$input v_FragPos

/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "../common/common.sh"

uniform vec4 viewPos;

uniform vec4 u_time;
SAMPLER2D(s_glass_diff, 0);

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
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // attenuation
    float distance = length(spotLight_position.xyz - fragPos);
    float attenuation = 1.0 / (spotLight_constant + spotLight_linear * distance + spotLight_quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-spotLight_direction.xyz));
    float epsilon = spotLight_cutOff - spotLight_outerCutOff;
    float intensity = clamp((theta - spotLight_outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = spotLight_ambient.xyz * vec3(texture2D(s_glass_diff, v_texcoord0));
    vec3 diffuse = spotLight_diffuse.xyz * diff * vec3(texture2D(s_glass_diff, v_texcoord0));
    vec3 specular = vec3(0.0);
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
    vec3 result = CalcSpotLight(norm, v_FragPos, viewDir, v_texcoord0);
    gl_FragColor = vec4(result, 1.0);
}
