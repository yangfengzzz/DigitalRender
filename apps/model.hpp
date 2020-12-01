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
    Model(std::string directory, aiNode *node, const aiScene *scene,
          Model* parent, const Shader& shader);
    
    ~Model();
    
    // draws the model, and thus all its meshes
    void draw() override;
    
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
