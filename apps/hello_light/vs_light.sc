$input a_position, a_normal, a_texcoord0
$output v_texcoord0, v_normal, v_FragPos

/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "../common/common.sh"

uniform mat4 normal_matrix;

void main()
{
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );
    v_FragPos = mul(u_model[0], vec4(a_position, 1.0) ).xyz;
    
    vec4 normal = a_normal * 2.0 - 1.0;
    v_normal = mul(normal_matrix, vec4(normal.xyz, 0.0) ).xyz;
    v_texcoord0 = vec2(a_texcoord0.x, 1.0 - a_texcoord0.y);
}
