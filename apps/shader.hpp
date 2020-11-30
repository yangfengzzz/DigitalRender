//
//  shader.hpp
//  apps
//
//  Created by 杨丰 on 2020/11/30.
//

#ifndef shader_hpp
#define shader_hpp

#include <string>
#include <bgfx/bgfx.h>
#include "common/bgfx_utils.h"

namespace vox {
class Shader {
public:
    Shader() {}
    
    void load(std::string vertexFunctionName,
         std::string fragmentFunctionName) {
        // Create program from shaders.
        m_program = loadProgram(vertexFunctionName.c_str(),
                                fragmentFunctionName.c_str());
        
        state = 0
        | BGFX_STATE_WRITE_R
        | BGFX_STATE_WRITE_G
        | BGFX_STATE_WRITE_B
        | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z
        | BGFX_STATE_DEPTH_TEST_LESS
        | BGFX_STATE_CULL_CW
        | BGFX_STATE_MSAA
        | UINT64_C(0)
        ;
    }
    
    ~Shader() {
        bgfx::destroy(m_program);
    }
    
public:
    bgfx::ProgramHandle m_program;
    uint64_t state = 0;
};

}
#endif /* shader_hpp */
