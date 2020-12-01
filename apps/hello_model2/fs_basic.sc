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


void main()
{
    gl_FragColor = u_time;
}
