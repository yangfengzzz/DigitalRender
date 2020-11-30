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

#include "mesh.hpp"

namespace vox {

class Model {
public:
    Model(std::string path) {
        loadModel(path);
    }
    
    // draws the model, and thus all its meshes
    void Draw(Shader &shader)
    {
        for(unsigned int i = 0; i < meshes.size(); i++) {
            // Set model matrix for rendering.
            bgfx::setTransform(glm::value_ptr(modelTransform));
            
            meshes[i].Draw(shader);
        }
    }
    
    void setTransform(glm::mat4 transform) {
        this->modelTransform = transform;
        
    }
    
private:
    // loads a model with supported ASSIMP extensions from file
    // and stores the resulting meshes in the meshes vector.
    void loadModel(std::string const &path);
    
    // processes a node in a recursive fashion.
    // Processes each individual mesh located at the node
    // and repeats this process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene *scene);
    
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    
    // checks all material textures of a given type and loads
    // the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat,
                                              aiTextureType type,
                                              std::string typeName);
    
public:
    std::string directory;
    std::vector<Mesh> meshes;
    // stores all the textures loaded so far,
    // optimization to make sure textures aren't loaded more than once.
    std::vector<Texture> textures_loaded;
    
    glm::mat4 modelTransform = glm::mat4(1.0f);
};

}
#endif /* model_hpp */
