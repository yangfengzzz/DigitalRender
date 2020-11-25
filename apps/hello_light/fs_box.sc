$input v_texcoord0
$input v_normal
$input v_FragPos

/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "../common/common.sh"

uniform vec4 objectColor;
uniform vec4 viewPos;

uniform vec4 material[4];
#define material_ambient  material[0]
#define material_diffuse material[1]
#define material_specular   material[2]
#define material_shininess   material[3]

uniform vec4 light[4];
#define lightPos  light[0]
#define light_ambient light[1]
#define light_diffuse   light[2]
#define light_specular   light[3]

void main()
{
    //Ambient
    vec4 ambient =  material_ambient * light_ambient;
    
    //Diffuse
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(vec3(lightPos) - v_FragPos);
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = (diff * material_diffuse) * light_diffuse;
    
    //Specular
    vec3 viewDir = normalize(vec3(viewPos) - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess.x);
    vec4 specular = material_specular * spec * light_specular;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    gl_FragColor = vec4(result, 1.0);
}
