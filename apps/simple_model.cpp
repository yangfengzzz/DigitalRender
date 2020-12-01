//
//  simple_model.cpp
//  apps
//
//  Created by 杨丰 on 2020/12/1.
//

#include "simple_model.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace vox {
void SimpleModel::draw() {
    // Set model matrix for rendering.
    bgfx::setTransform(glm::value_ptr(modelTransform));
    // Set instance data buffer.
    if (getInstanceCount() != 1) {
        bgfx::setInstanceDataBuffer(&getInstanceDataBuffer());
    }
    
    // Set vertex and index buffer.
    bgfx::setVertexBuffer(0, m_vbh);
    bgfx::setIndexBuffer(m_ibh);
    
    // Bind textures.
    for (size_t i = 0; i < textures.size(); i++) {
        bgfx::setTexture(i, samples[i], textures[i]);
    }
    
    // Set render states.
    bgfx::setState(shader.state);
    
    // Submit primitive for rendering to view 0.
    bgfx::submit(0, shader.programs);
}
}
