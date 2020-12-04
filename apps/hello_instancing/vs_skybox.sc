$input a_position, a_normal, a_texcoord0
$output v_view

/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "../common/common.sh"

void main()
{
    v_view = a_position;
    mat4 view = u_view;
    view[0][3] = 0;
    view[1][3] = 0;
    view[2][3] = 0;
    view[3][3] = 1;
    vec4 pos = mul(u_proj, mul(view, vec4(a_position, 1.0)));
    gl_Position = pos.xyww;
}
