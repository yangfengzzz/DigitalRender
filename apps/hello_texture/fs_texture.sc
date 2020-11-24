$input v_color0, v_texcoord0

/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "../common/common.sh"

SAMPLER2D(s_texColor1,  0);
SAMPLER2D(s_texColor2,  1);

void main()
{
	gl_FragColor = mix(texture2D(s_texColor1, v_texcoord0), texture2D(s_texColor2, v_texcoord0), 0.2);
}
