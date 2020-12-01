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
#include <assimp/scene.h>
#include "shader.hpp"

namespace vox {
class Model;
struct Texture {
    bgfx::TextureHandle m_texture;
    std::string type;
    std::string path;
};

class Mesh {
public:
    void load(aiMesh* mesh, aiMaterial* material, Model* parent);
    
    // render the mesh
    void draw(Shader &shader)
    {
        // Set vertex and index buffer.
        bgfx::setVertexBuffer(0, m_vbh);
        bgfx::setIndexBuffer(m_ibh);
        
        // Bind textures.
        for (size_t i = 0; i < textures.size(); i++) {
            bgfx::setTexture(i, samples[i], textures[i].m_texture);
        }
        
        // Set render states.
        bgfx::setState(shader.getState());
        
        // Submit primitive for rendering to view 0.
        bgfx::submit(0, shader.getProgram(static_cast<int>(textures.size())));
    }
    
private:
    void processMesh();
    
    // checks all material textures of a given type and loads
    // the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat,
                                              aiTextureType type,
                                              std::string typeName);
    
    // initializes all the buffer objects/arrays
    // Static data can be passed with bgfx::makeRef
    void setupMesh();
    
public:
    aiMesh* mesh;
    aiMaterial* material;
    Model* parent;
    
    std::vector<float> vertices;
    std::vector<uint16_t> indices;
    bgfx::VertexLayout ms_layout;
    
    bgfx::VertexBufferHandle m_vbh;
    bgfx::IndexBufferHandle m_ibh;
    
    std::vector<Texture> textures;
    std::vector<bgfx::UniformHandle> samples;
};

}

#endif /* mesh_hpp */
