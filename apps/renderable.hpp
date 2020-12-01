//
//  renderable.hpp
//  apps
//
//  Created by 杨丰 on 2020/12/1.
//

#ifndef renderable_hpp
#define renderable_hpp

#include "shader.hpp"

namespace vox {
class Renderable {
public:
    Renderable(const Shader& shader) : shader(shader){}
    
    virtual void draw() = 0;
    
    Shader shader;
    void reloadShader(const Shader& m_shader) {
        shader = m_shader;
    }
};

}
#endif /* renderable_hpp */
