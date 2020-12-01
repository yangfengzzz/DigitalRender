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

void main()
{
    gl_FragColor = texture2D(s_texColor0, v_texcoord0);
}
