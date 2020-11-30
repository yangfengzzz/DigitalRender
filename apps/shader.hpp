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
    Shader(std::string vertexFunctionName,
           std::string fragmentFunctionName) {
        // Create program from shaders.
        m_program = loadProgram(vertexFunctionName.c_str(),
                                fragmentFunctionName.c_str());
        
        state = 0
        | BGFX_STATE_WRITE_Z
        | BGFX_STATE_DEPTH_TEST_LESS
        | BGFX_STATE_CULL_CW
        | BGFX_STATE_MSAA
        | UINT64_C(0)
        ;
    }
    
public:
    bgfx::ProgramHandle m_program;
    uint64_t state = 0;
};

}
#endif /* shader_hpp */
