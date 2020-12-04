//
//  skybox.hpp
//  apps
//
//  Created by 杨丰 on 2020/12/4.
//

#ifndef skybox_hpp
#define skybox_hpp

#include "node.hpp"
#include "simple_mesh.hpp"
#include "bgfx_utils.h"

namespace vox {
class Skybox : public Node {
    void init() {
        ms_layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal,   3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .end();
        
        // Create static vertex buffer.
        m_vbh = bgfx::createVertexBuffer(
                                         // Static data can be passed with bgfx::makeRef
                                         bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices) )
                                         , ms_layout
                                         );
        
        // Create static index buffer for triangle list rendering.
        m_ibh = bgfx::createIndexBuffer(
                                        // Static data can be passed with bgfx::makeRef
                                        bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList) )
                                        );
        
        shader.loadShader("../../../hello_instancing/vs_skybox",
                          "../../../hello_instancing/fs_skybox");
        s_texCube = bgfx::createUniform("s_texCube",  bgfx::UniformType::Sampler);
    }
    
    void loadTexture(std::string textureName) {
        m_texture = ::loadTexture(textureName.c_str());
    }
    
    void draw() {
        // Set vertex and index buffer.
        bgfx::setVertexBuffer(0, m_vbh);
        bgfx::setIndexBuffer(m_ibh);
        
        // Bind textures.
        bgfx::setTexture(0, s_texCube, m_texture);
        
        // Set render states.
        bgfx::setState(shader.state);
        
        // Submit primitive for rendering to view 0.
        bgfx::submit(0, shader.programs);
    }
    
public:
    bgfx::VertexLayout ms_layout;
    bgfx::VertexBufferHandle m_vbh;
    bgfx::IndexBufferHandle m_ibh;
    
    Shader shader;
    bgfx::TextureHandle m_texture;
    bgfx::UniformHandle s_texCube;
};

}
#endif /* skybox_hpp */
