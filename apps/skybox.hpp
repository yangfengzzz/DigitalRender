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
public:
    void init(std::string textureName) {
        ms_layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .end();
        
        // Create static vertex buffer.
        m_vbh = bgfx::createVertexBuffer(
                                         // Static data can be passed with bgfx::makeRef
                                         bgfx::makeRef(skyboxVertices, sizeof(skyboxVertices) )
                                         , ms_layout
                                         );
        
        // Create static index buffer for triangle list rendering.
        m_ibh = bgfx::createIndexBuffer(
                                        // Static data can be passed with bgfx::makeRef
                                        bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList) )
                                        );
        
        programs = loadProgram("../../../hello_instancing/vs_skybox",
                               "../../../hello_instancing/fs_skybox");
        s_texCube = bgfx::createUniform("s_texCube",  bgfx::UniformType::Sampler);
        
        m_texture = loadTexture(textureName.c_str()
                                , 0
                                | BGFX_SAMPLER_U_CLAMP
                                | BGFX_SAMPLER_V_CLAMP
                                | BGFX_SAMPLER_W_CLAMP
                                );
    }
    
    void draw() {
        // Set vertex and index buffer.
        bgfx::setVertexBuffer(0, m_vbh);
        bgfx::setIndexBuffer(m_ibh);
        
        // Bind textures.
        bgfx::setTexture(0, s_texCube, m_texture);
        
        // Set render states.
        bgfx::setState(BGFX_STATE_WRITE_RGB|BGFX_STATE_WRITE_A
                       | BGFX_STATE_DEPTH_TEST_LEQUAL
                       | BGFX_STATE_MSAA);
        
        // Submit primitive for rendering to view 0.
        bgfx::submit(0, programs);
    }
    
private:
    bgfx::VertexLayout ms_layout;
    bgfx::VertexBufferHandle m_vbh;
    bgfx::IndexBufferHandle m_ibh;
    
    bgfx::ProgramHandle programs;
    bgfx::TextureHandle m_texture;
    bgfx::UniformHandle s_texCube;
};

}
#endif /* skybox_hpp */
