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
#include "texture.hpp"

namespace vox {
class Model;

class Mesh {
public:
    void load(aiMesh* mesh, aiMaterial* material, Model* parent);
    
    // render the mesh
    void draw();
    
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
