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

uniform vec4 light[10];
#define lightPos  light[0]
#define light_ambient light[1]
#define light_diffuse   light[2]
#define light_specular   light[3]
#define light_constant   light[4]
#define light_linear   light[5]
#define light_quadratic   light[6]
#define light_direction  light[7]
#define light_cutOff  light[8]
#define light_outerCutOff  light[9]

void main()
{
    //Ambient
    vec4 ambient =  texture2D(s_diffuse, v_texcoord0) * light_ambient;
    
    //Diffuse
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(vec3(lightPos) - v_FragPos);
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = (diff * texture2D(s_diffuse, v_texcoord0)) * light_diffuse;
    
    //Specular
    vec3 viewDir = normalize(vec3(viewPos) - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess.x);
    vec4 specular = texture2D(s_specular, v_texcoord0) * spec * light_specular;

    //Spot light(soft edge)
    float theta = dot(lightDir, normalize(-light_direction));
    float epsilon   = light_cutOff.x - light_outerCutOff.x;
    float intensity = clamp((theta - light_outerCutOff.x) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;
    
    // attenuation
    float distance    = length(lightPos - v_FragPos);
    float attenuation = 1.0 / (light_constant.x + light_linear.x * distance + light_quadratic.x * (distance * distance));
    
    gl_FragColor = (ambient + diffuse + specular) * attenuation;
}
