//
//  nodeFactory.hpp
//  apps
//
//  Created by 杨丰 on 2020/12/1.
//

#ifndef nodeFactory_hpp
#define nodeFactory_hpp

#include "node.hpp"
#include "simple_model.hpp"

namespace vox {
static float planeVertices[] = {
    // positions            // normals         // texcoords
    10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
    -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
    -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
    
    10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
    -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
    10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
};

static const uint16_t s_cubeTriList[] =
{
    2,  1,  0,
    5,  4,  3,
};

class NodeFactory {
public:
    std::shared_ptr<Node> createBox(){
        bgfx::VertexLayout ms_layout;
        ms_layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal,   3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .end();
        
        // Create static vertex buffer.
        bgfx::VertexBufferHandle m_vbh
        = bgfx::createVertexBuffer(
                                   // Static data can be passed with bgfx::makeRef
                                   bgfx::makeRef(planeVertices, sizeof(planeVertices) )
                                   , ms_layout
                                   );
        
        // Create static index buffer for triangle list rendering.
        bgfx::IndexBufferHandle m_ibh
        = bgfx::createIndexBuffer(
                                  // Static data can be passed with bgfx::makeRef
                                  bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList) )
                                  );
        
        shader.loadShader("../../../hello_model2/vs_model",
                          "../../../hello_model2/fs_basic");
        
        std::shared_ptr<SimpleModel> box = std::make_shared<SimpleModel>(ms_layout, m_vbh, m_ibh, shader);
        bgfx::TextureHandle m_diffuse = loadTexture("/Users/yangfeng/Desktop/DigitalRender/apps/Models/Textures/wood.png");
        box->addTexture(m_diffuse);
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
        box->setTransform(model);
        return box;
    }
    
private:
    Shader shader;
};

}

#endif /* nodeFactory_hpp */
