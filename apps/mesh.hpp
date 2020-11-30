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
    }
    
    // render the mesh
    void Draw(bgfx::ProgramHandle &shader)
    {
        
    }
    
public:
    std::vector<float>        vertices;
    std::vector<unsigned int> indices;
    bgfx::VertexLayout ms_layout;
    
    std::vector<Texture>      textures;
};

#endif /* mesh_hpp */
