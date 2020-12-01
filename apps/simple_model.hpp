//
//  simple_model.hpp
//  apps
//
//  Created by 杨丰 on 2020/12/1.
//

#ifndef simple_model_hpp
#define simple_model_hpp

#include "node.hpp"
#include "renderable.hpp"
#include "texture.hpp"

namespace vox {
class SimpleModel : public Node, public Renderable {
public:
    SimpleModel(bgfx::VertexLayout ms_layout,
                bgfx::VertexBufferHandle m_vbh,
                bgfx::IndexBufferHandle m_ibh,
                const Shader& shader):
    Renderable(shader),
    ms_layout(ms_layout),
    m_vbh(m_vbh),
    m_ibh(m_ibh) {
        this->renderable = this;
    }
    
    void addTexture(bgfx::TextureHandle texture) {
        std::string samples_name = "s_texColor" + std::to_string(textures.size());
        bgfx::UniformHandle s_texColor1  = bgfx::createUniform(samples_name.c_str(),
                                                               bgfx::UniformType::Sampler);
        samples.push_back(s_texColor1);
        textures.push_back(texture);
    }
    
    void draw() override {
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
    
private:
    bgfx::VertexLayout ms_layout;
    
    bgfx::VertexBufferHandle m_vbh;
    bgfx::IndexBufferHandle m_ibh;
    
    std::vector<bgfx::TextureHandle> textures;
    std::vector<bgfx::UniformHandle> samples;    
};

}

#endif /* simple_model_hpp */
