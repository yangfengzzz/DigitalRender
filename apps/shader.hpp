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
    }
    
public:
    bgfx::ProgramHandle m_program;
};

}
#endif /* shader_hpp */
