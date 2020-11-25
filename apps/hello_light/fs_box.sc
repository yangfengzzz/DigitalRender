$input v_texcoord0
$input v_normal
$input v_FragPos

/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "../common/common.sh"

uniform vec4 objectColor;
uniform vec4 lightColor;
uniform vec4 lightPos;
uniform vec4 viewPos;

void main()
{
    //Diffuse
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(vec3(lightPos) - v_FragPos);
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = diff * lightColor;

    //Ambient
    float ambientStrength = 0.1;
    vec4 ambient = ambientStrength * lightColor;
    
    //Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(vec3(viewPos) - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec4 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    gl_FragColor = vec4(result, 1.0);
}
