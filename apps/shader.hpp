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
class Shader {
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
    
    void addShader(int index, std::string vertexFunctionName,
                   std::string fragmentFunctionName) {
        // Create program from shaders.
        bgfx::ProgramHandle m_shader = loadProgram(vertexFunctionName.c_str(),
                                                   fragmentFunctionName.c_str());
        
        programs.insert(std::unordered_map<int, bgfx::ProgramHandle>::value_type(index, m_shader));
    }
    
    uint64_t getState() {
        return state;
    }
    
    bgfx::ProgramHandle getProgram(int index) {
        std::unordered_map<int, bgfx::ProgramHandle>::const_iterator it = programs.find(index);
        if (it != programs.end()) {
            return it->second;
        } else {
            return BGFX_INVALID_HANDLE;
        }
    }
    
    ~Shader() {
        for (auto& shader : programs) {
            bgfx::destroy(shader.second);
        }
    }
    
private:
    std::unordered_map<int, bgfx::ProgramHandle> programs;
    uint64_t state = 0;
};

}
#endif /* shader_hpp */
