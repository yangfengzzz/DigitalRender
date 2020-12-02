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
                Node* parent,
                const Shader& shader):
    Renderable(shader),
    ms_layout(ms_layout),
    m_vbh(m_vbh),
    m_ibh(m_ibh) {
        this->parent = parent;
        this->renderable = this;
    }
    
    void addTexture(bgfx::TextureHandle texture) {
        std::string samples_name = "s_texColor" + std::to_string(textures.size());
        bgfx::UniformHandle s_texColor1  = bgfx::createUniform(samples_name.c_str(),
                                                               bgfx::UniformType::Sampler);
        samples.push_back(s_texColor1);
        textures.push_back(texture);
    }
    
    void draw() override;
    
private:
    bgfx::VertexLayout ms_layout;
    
    bgfx::VertexBufferHandle m_vbh;
    bgfx::IndexBufferHandle m_ibh;
    
    std::vector<bgfx::TextureHandle> textures;
    std::vector<bgfx::UniformHandle> samples;    
};

}

#endif /* simple_model_hpp */
