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

namespace vox {
Model::Model(std::string directory, aiNode *node, const aiScene *scene,
             Model* parent, const Shader& shader):
Renderable(shader),
directory(directory),
node(node),
scene(scene) {
    this->parent = parent;
    this->name = std::string(node->mName.C_Str());
    this->renderable = this;
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        childNodes.push_back(std::make_shared<Model>(directory, node->mChildren[i],
                                                     scene, this, shader));
    }
    
    processNode();
}

Model::~Model() {
    for (size_t i = 0; i < meshes.size(); i++) {
        bgfx::destroy(meshes[i].m_vbh);
        bgfx::destroy(meshes[i].m_ibh);
        for (size_t j = 0; j < meshes[i].samples.size(); j++) {
            bgfx::destroy(meshes[i].samples[j]);
            bgfx::destroy(meshes[i].textures[j].m_texture);
        }
    }
}

// draws the model, and thus all its meshes
void Model::draw()
{
    for (size_t i = 0; i < childNodes.size(); i++) {
        if (childNodes[i]->renderable != nullptr) {
            childNodes[i]->renderable->draw();
        }
    }
        
    for(unsigned int i = 0; i < meshes.size(); i++) {
        // Set model matrix for rendering.
        bgfx::setTransform(glm::value_ptr(modelTransform));
        // Set instance data buffer.
        if (getInstanceCount() != 1) {
            bgfx::setInstanceDataBuffer(&getInstanceDataBuffer());
        }
        
        meshes[i].draw();
    }
}

void Model::processNode() {
    meshes.resize(node->mNumMeshes);
    // process each mesh located at the current node
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene.
        // the scene contains all the data,
        // node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        meshes[i].load(mesh, material, this);
    }
}

}
