//
//  scene.hpp
//  apps
//
//  Created by 杨丰 on 2020/12/1.
//

#ifndef scene_hpp
#define scene_hpp

#include <assimp/Importer.hpp>
#include <iostream>
#include "node.hpp"
#include "shader.hpp"

namespace vox {
class Scene {
public:
    void loadAssimp(std::string const &path);
    
    // draws the model, and thus all its meshes
    void draw(Shader &shader)
    {
        rootNode->draw(shader);
    }
    
    void setTransform(glm::mat4 transform) {
        rootNode->localTransform = transform;
        
    }
    
    std::shared_ptr<Node> getRoot(){
        return rootNode;
    }
    
private:
    std::shared_ptr<Node> rootNode;
    Assimp::Importer importer;
};

}

#endif /* scene_hpp */
