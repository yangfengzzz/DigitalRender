$input v_texcoord0
$input v_normal
$input v_FragPos

/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "../common/common.sh"

uniform vec4 viewPos;

SAMPLER2D(s_texColor0, 0);

uniform vec4 dirLight[4];
#define dirLight_direction dirLight[0]
#define dirLight_ambient dirLight[1]
#define dirLight_diffuse dirLight[2]
#define dirLight_specular dirLight[3]
vec3 CalcDirLight(vec3 normal, vec3 viewDir, vec2 v_texcoord0)
{
    vec3 color = texture2D(s_texColor0, v_texcoord0).rgb;
    
    vec3 lightDir = normalize(-dirLight_direction.xyz);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    // 合并结果
    vec3 ambient  = dirLight_ambient.xyz  * color;
    vec3 diffuse  = dirLight_diffuse.xyz  * diff * color;
    vec3 specular = dirLight_specular.xyz * spec;
    return (ambient + diffuse + specular);
}

void main()
{
    // properties
    vec3 norm = normalize(v_normal);
    vec3 viewDir = normalize(vec3(viewPos) - v_FragPos);
    
    gl_FragColor = vec4(CalcDirLight(norm, viewDir, v_texcoord0), 1.0);
}
