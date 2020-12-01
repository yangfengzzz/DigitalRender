//
//  mesh.cpp
//  apps
//
//  Created by 杨丰 on 2020/11/29.
//

#include "mesh.hpp"
#include "model.hpp"

namespace vox {
void Mesh::load(aiMesh* m_mesh, aiMaterial* m_material, Model* m_parent) {
    this->mesh = m_mesh;
    this->material = m_material;
    this->parent = m_parent;
    processMesh();
}

void Mesh::processMesh()
{
    //clear memory
//    vertices.clear();
//    indices.clear();
//    for (size_t i = 0; i < textures.size(); i++) {
//        bgfx::destroy(textures[i].m_texture);
//        bgfx::destroy(samples[i]);
//    }
//    textures.clear();
//    samples.clear();
    
    // alloc memory first
    int sizeofVertex = 3;
    ms_layout.begin()
    .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float);
    if (mesh->HasNormals()) {
        sizeofVertex += 3;
        ms_layout.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float);
    }
    if(mesh->mTextureCoords[0]) {
        sizeofVertex += 8;
        ms_layout.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Bitangent, 3, bgfx::AttribType::Float);
    }
    ms_layout.end();
    vertices.reserve(sizeofVertex * mesh->mNumVertices);
    
    // walk through each of the mesh's vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        // positions
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);
        // normals
        if (mesh->HasNormals())
        {
            vertices.push_back(mesh->mNormals[i].x);
            vertices.push_back(mesh->mNormals[i].y);
            vertices.push_back(mesh->mNormals[i].z);
        }
        // texture coordinates
        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            // a vertex can contain up to 8 different texture coordinates.
            // We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates
            // so we always take the first set (0).
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
            // tangent
            vertices.push_back(mesh->mTangents[i].x);
            vertices.push_back(mesh->mTangents[i].y);
            vertices.push_back(mesh->mTangents[i].y);
            // bitangent
            vertices.push_back(mesh->mBitangents[i].x);
            vertices.push_back(mesh->mBitangents[i].y);
            vertices.push_back(mesh->mBitangents[i].y);
        }
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle)
    // and retrieve the corresponding vertex indices.
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        indices.push_back(face.mIndices[j]);
    }
    
    setupMesh();
    
    // process materials
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN
    
    // 1. diffuse maps
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    
    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    samples.reserve(textures.size());
    for (size_t i = 0; i < textures.size(); i++) {
        std::string samples_name = "s_texColor" + std::to_string(i);
        bgfx::UniformHandle s_texColor1  = bgfx::createUniform(samples_name.c_str(),
                                                               bgfx::UniformType::Sampler);
        samples.push_back(s_texColor1);
    }
}

std::vector<Texture> Mesh::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures_tmp;
    
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for(unsigned int j = 0; j < parent->textures_loaded.size(); j++)
        {
            if(std::strcmp(parent->textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures_tmp.push_back(parent->textures_loaded[j]);
                // a texture with the same filepath has already been loaded,
                // continue to next one. (optimization)
                skip = true;
                break;
            }
        }
        if(!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.m_texture = loadTexture(std::string(parent->directory + '/' + std::string(str.C_Str())).c_str());
            texture.type = typeName;
            texture.path = str.C_Str();
            textures_tmp.push_back(texture);
            // store it as texture loaded for entire model,
            // to ensure we won't unnecesery load duplicate textures.
            parent->textures_loaded.push_back(texture);
        }
    }
    return textures_tmp;
}

void Mesh::setupMesh()
{
    // Create static vertex buffer.
    m_vbh = bgfx::createVertexBuffer(bgfx::makeRef(&(vertices[0]),
                                                   sizeof(float) * static_cast<uint32_t>(vertices.size()) )
                                     , ms_layout);
    
    // Create static index buffer for triangle list rendering.
    m_ibh = bgfx::createIndexBuffer(bgfx::makeRef(&(indices[0]),
                                                  sizeof(uint16_t) * static_cast<uint32_t>(indices.size()) ));
}

}
