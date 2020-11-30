//
//  model.cpp
//  apps
//
//  Created by 杨丰 on 2020/11/30.
//

#include "model.hpp"
#include <bx/pixelformat.h>
#include <bx/commandline.h>
#include <bx/endian.h>
#include <bx/math.h>
#include <bx/readerwriter.h>
#include <bx/string.h>
#include <bimg/decode.h>
#include "common/entry/entry.h"

namespace {
void* load(bx::FileReaderI* _reader, bx::AllocatorI* _allocator, const char* _filePath, uint32_t* _size)
{
    if (bx::open(_reader, _filePath) )
    {
        uint32_t size = (uint32_t)bx::getSize(_reader);
        void* data = BX_ALLOC(_allocator, size);
        bx::read(_reader, data, size);
        bx::close(_reader);
        if (NULL != _size)
        {
            *_size = size;
        }
        return data;
    }
    else
    {
        DBG("Failed to open: %s.", _filePath);
    }
    
    if (NULL != _size)
    {
        *_size = 0;
    }
    
    return NULL;
}

void* load(const char* _filePath, uint32_t* _size)
{
    return load(entry::getFileReader(), entry::getAllocator(), _filePath, _size);
}

void unload(void* _ptr)
{
    BX_FREE(entry::getAllocator(), _ptr);
}

static void imageReleaseCb(void* _ptr, void* _userData)
{
    BX_UNUSED(_ptr);
    bimg::ImageContainer* imageContainer = (bimg::ImageContainer*)_userData;
    bimg::imageFree(imageContainer);
}

}

//MARK:- Namespace Vox
namespace vox {
bgfx::TextureHandle loadTexture(bx::FileReaderI* _reader,
                                const char* _filePath, uint64_t _flags, uint8_t _skip,
                                bgfx::TextureInfo* _info, bimg::Orientation::Enum* _orientation)
{
    BX_UNUSED(_skip);
    bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;
    
    uint32_t size;
    void* data = load(_reader, entry::getAllocator(), _filePath, &size);
    if (NULL != data)
    {
        bimg::ImageContainer* imageContainer = bimg::imageParse(entry::getAllocator(), data, size);
        
        if (NULL != imageContainer)
        {
            if (NULL != _orientation)
            {
                *_orientation = imageContainer->m_orientation;
            }
            
            const bgfx::Memory* mem = bgfx::makeRef(
                                                    imageContainer->m_data
                                                    , imageContainer->m_size
                                                    , imageReleaseCb
                                                    , imageContainer
                                                    );
            unload(data);
            
            if (imageContainer->m_cubeMap)
            {
                handle = bgfx::createTextureCube(
                                                 uint16_t(imageContainer->m_width)
                                                 , 1 < imageContainer->m_numMips
                                                 , imageContainer->m_numLayers
                                                 , bgfx::TextureFormat::Enum(imageContainer->m_format)
                                                 , _flags
                                                 , mem
                                                 );
            }
            else if (1 < imageContainer->m_depth)
            {
                handle = bgfx::createTexture3D(
                                               uint16_t(imageContainer->m_width)
                                               , uint16_t(imageContainer->m_height)
                                               , uint16_t(imageContainer->m_depth)
                                               , 1 < imageContainer->m_numMips
                                               , bgfx::TextureFormat::Enum(imageContainer->m_format)
                                               , _flags
                                               , mem
                                               );
            }
            else if (bgfx::isTextureValid(0, false, imageContainer->m_numLayers, bgfx::TextureFormat::Enum(imageContainer->m_format), _flags) )
            {
                handle = bgfx::createTexture2D(
                                               uint16_t(imageContainer->m_width)
                                               , uint16_t(imageContainer->m_height)
                                               , 1 < imageContainer->m_numMips
                                               , imageContainer->m_numLayers
                                               , bgfx::TextureFormat::Enum(imageContainer->m_format)
                                               , _flags
                                               , mem
                                               );
            }
            
            if (bgfx::isValid(handle) )
            {
                bgfx::setName(handle, _filePath);
            }
            
            if (NULL != _info)
            {
                bgfx::calcTextureSize(
                                      *_info
                                      , uint16_t(imageContainer->m_width)
                                      , uint16_t(imageContainer->m_height)
                                      , uint16_t(imageContainer->m_depth)
                                      , imageContainer->m_cubeMap
                                      , 1 < imageContainer->m_numMips
                                      , imageContainer->m_numLayers
                                      , bgfx::TextureFormat::Enum(imageContainer->m_format)
                                      );
            }
        }
    }
    
    return handle;
}

bgfx::TextureHandle loadTexture(const char* _name, uint64_t _flags, uint8_t _skip,
                                bgfx::TextureInfo* _info, bimg::Orientation::Enum* _orientation)
{
    return loadTexture(entry::getFileReader(), _name, _flags, _skip, _info, _orientation);
}

//MARK:- Class Model
void Model::loadModel(std::string const &path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
                                             aiProcess_Triangulate |
                                             aiProcess_GenSmoothNormals |
                                             aiProcess_FlipUVs |
                                             aiProcess_CalcTangentSpace);
    // check for errors
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));
    
    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // process each mesh located at the current node
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene.
        // the scene contains all the data,
        // node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any)
    // we then recursively process each of the children nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    // data to fill
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    bgfx::VertexLayout ms_layout;
    
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
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
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
    
    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, ms_layout, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                // a texture with the same filepath has already been loaded,
                // continue to next one. (optimization)
                skip = true;
                break;
            }
        }
        if(!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.m_texture = loadTexture(std::string(directory + '/' + std::string(str.C_Str())).c_str());
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            // store it as texture loaded for entire model,
            // to ensure we won't unnecesery load duplicate textures.
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

}
