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
#include "simple_mesh.hpp"

namespace vox {
class NodeFactory {
public:
    std::shared_ptr<Node> createPlane(Node* parent){
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
                                  bgfx::makeRef(s_planeTriList, sizeof(s_planeTriList) )
                                  );
        
        shader.loadShader("../../../hello_instancing/vs_instancing",
                          "../../../hello_instancing/fs_blinn");
        
        std::shared_ptr<SimpleModel> box = std::make_shared<SimpleModel>(ms_layout, m_vbh, m_ibh,
                                                                         parent, shader);
        bgfx::TextureHandle m_diffuse = loadTexture("/Users/yangfeng/Desktop/DigitalRender/apps/Models/Textures/wood.png");
        box->addTexture(m_diffuse);
        
        box->scale = glm::vec3(10.0f, 10.0f, 10.0f);
        return box;
    }
    
private:
    Shader shader;
};

}

#endif /* nodeFactory_hpp */
