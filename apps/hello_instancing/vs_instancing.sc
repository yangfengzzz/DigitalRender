$input a_position, a_normal, a_texcoord0, i_data0, i_data1, i_data2, i_data3
$output v_texcoord0, v_normal, v_FragPos

/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "../common/common.sh"

void main()
{
    mat4 model;
    model[0] = i_data0;
    model[1] = i_data1;
    model[2] = i_data2;
    model[3] = i_data3;
    
    vec4 worldPos = instMul(model, vec4(a_position, 1.0) );
    gl_Position = mul(u_modelViewProj, worldPos);
    v_FragPos = mul(u_model[0], worldPos).xyz;
    
    v_normal = mul(u_model[0], mul(model, a_normal));
    v_texcoord0 = a_texcoord0;
}
