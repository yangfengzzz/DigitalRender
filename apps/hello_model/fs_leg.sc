$input v_texcoord0

/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "../common/common.sh"

uniform vec4 u_time;
SAMPLER2D(s_leg_diff, 0);
SAMPLER2D(s_leg_spec, 2);

void main()
{
	gl_FragColor = texture2D(s_leg_diff, v_texcoord0);
}
