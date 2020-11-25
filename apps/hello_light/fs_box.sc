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

    vec3 result = (ambient + diffuse) * objectColor;
    gl_FragColor = vec4(result, 1.0);
}
