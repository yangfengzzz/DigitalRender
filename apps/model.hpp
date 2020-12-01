//
//  model.hpp
//  apps
//
//  Created by 杨丰 on 2020/11/30.
//

#ifndef model_hpp
#define model_hpp

#include <vector>
#include <string>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <bimg/bimg.h>
#include <glm/gtc/type_ptr.hpp>

#include "node.hpp"
#include "renderable.hpp"
#include "mesh.hpp"

namespace vox {

class Model: public Node, public Renderable {
public:
    Model(std::string directory, aiNode *node, const aiScene *scene, Model* parent, const Shader& shader):
    Renderable(shader),
    directory(directory),
    node(node),
    scene(scene) {
        this->parent = parent;
        this->name = std::string(node->mName.C_Str());
        this->renderable = this;
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            childNodes.push_back(std::make_shared<Model>(directory, node->mChildren[i], scene, this, shader));
        }
        
        processNode();
    }
    
    ~Model() {
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
    void draw() override
    {
        for (size_t i = 0; i < childNodes.size(); i++) {
            if (childNodes[i]->renderable != nullptr) {
                childNodes[i]->renderable->draw();
            }
        }
        
        update();
        
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
    
    void update() {
        if (parent != nullptr) {
            modelTransform = modelTransform * localTransform;
        } else {
            modelTransform = localTransform;
        }
    }
    
private:
    // processes a node in a recursive fashion.
    // Processes each individual mesh located at the node
    // and repeats this process on its children nodes (if any).
    void processNode();
    
public:
    std::string directory;
    aiNode *node;
    const aiScene *scene;
    
    std::vector<Mesh> meshes;
    // stores all the textures loaded so far,
    // optimization to make sure textures aren't loaded more than once.
    std::vector<Texture> textures_loaded;
};

}
#endif /* model_hpp */
