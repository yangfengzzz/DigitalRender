$input v_texcoord0

/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "../common/common.sh"

uniform vec4 objectColor;
uniform vec4 lightColor;

void main()
{
	gl_FragColor = lightColor * objectColor;
}
