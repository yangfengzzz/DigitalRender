//
//  mesh.hpp
//  apps
//
//  Created by 杨丰 on 2020/11/29.
//

#ifndef mesh_hpp
#define mesh_hpp

#include <glm/glm.hpp>
#include <bgfx/bgfx.h>
#include <vector>
#include <string>
#include "shader.hpp"

namespace vox {
struct Texture {
    bgfx::TextureHandle m_texture;
    std::string type;
    std::string path;
};

class Mesh {
public:
    Mesh(std::vector<float> vertices,
         std::vector<unsigned int> indices,
         bgfx::VertexLayout ms_layout,
         std::vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->ms_layout = ms_layout;
        this->textures = textures;
        
        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
        samples.reserve(textures.size());
        for (size_t i = 0; i < textures.size(); i++) {
            std::string samples_name = "s_texColor" + std::to_string(i);
            bgfx::UniformHandle s_texColor1  = bgfx::createUniform(samples_name.c_str(),
                                                                   bgfx::UniformType::Sampler);
            samples.push_back(s_texColor1);
        }
    }
    
    // render the mesh
    void Draw(Shader &shader)
    {
        // Set vertex and index buffer.
        bgfx::setVertexBuffer(0, m_vbh);
        bgfx::setIndexBuffer(m_ibh);
        
        // Bind textures.
        for (size_t i = 0; i < textures.size(); i++) {
            bgfx::setTexture(i, samples[i], textures[i].m_texture);
        }
        
        // Set render states.
        bgfx::setState(shader.state);
        
        // Submit primitive for rendering to view 0.
        bgfx::submit(0, shader.m_program);
    }
    
private:
    // initializes all the buffer objects/arrays
    // Static data can be passed with bgfx::makeRef
    void setupMesh()
    {
        // Create static vertex buffer.
        m_vbh = bgfx::createVertexBuffer(bgfx::makeRef(&(vertices[0]),
                                                       sizeof(float) * static_cast<uint32_t>(vertices.size()) )
                                         , ms_layout);
        
        // Create static index buffer for triangle list rendering.
        m_ibh = bgfx::createIndexBuffer(bgfx::makeRef(&(indices[0]),
                                                      sizeof(unsigned int) * static_cast<uint32_t>(indices.size()) ));
    }
    
public:
    std::vector<float>        vertices;
    std::vector<unsigned int> indices;
    bgfx::VertexLayout ms_layout;
    
    bgfx::VertexBufferHandle m_vbh;
    bgfx::IndexBufferHandle m_ibh;
    
    std::vector<Texture> textures;
    std::vector<bgfx::UniformHandle> samples;
};

}

#endif /* mesh_hpp */
