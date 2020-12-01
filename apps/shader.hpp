//
//  shader.hpp
//  apps
//
//  Created by 杨丰 on 2020/11/30.
//

#ifndef shader_hpp
#define shader_hpp

#include <string>
#include <unordered_map>
#include <bgfx/bgfx.h>
#include "common/bgfx_utils.h"

namespace vox {
struct Shader {
public:
    Shader() {
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
    
    void loadShader(std::string vertexFunctionName,
                    std::string fragmentFunctionName) {
        programs = loadProgram(vertexFunctionName.c_str(),
                               fragmentFunctionName.c_str());
    }
    
    ~Shader() {
        bgfx::destroy(programs);
    }
    
    bgfx::ProgramHandle programs = BGFX_INVALID_HANDLE;
    uint64_t state = 0;
};

}
#endif /* shader_hpp */
