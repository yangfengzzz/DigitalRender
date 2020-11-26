$input a_position, a_normal, a_texcoord0
$output v_texcoord0

/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "../common/common.sh"

uniform vec4 u_time;

void main()
{
	vec3 pos = a_position;
    gl_Position = mul(u_modelViewProj, vec4(pos, 1.0) );
    
    v_texcoord0 = a_texcoord0;
}
