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
    void loadAssimp(std::string const &path, const Shader& shader);
    
    // draws the model, and thus all its meshes
    void draw()
    {
        rootNode->draw();
    }
    
    void setTransform(glm::mat4 transform) {
        rootNode->localTransform = transform;
        
    }
    
    std::shared_ptr<Node> getRoot(){
        return rootNode;
    }
    
    std::shared_ptr<Node> findNode(std::string name,
                                   std::shared_ptr<Node> starter = nullptr) {
        if (starter == nullptr) {
            starter = rootNode;
        }
        
        //check self
        if (starter->name == name) {
            return starter;
        }
        
        //check childNode
        for (int i = 0; i < starter->childNodes.size(); i++) {
            if (starter->childNodes[i]->name == name) {
                return starter->childNodes[i];
            }
        }
        
        //check subchild
        for (int i = 0; i < starter->childNodes.size(); i++) {
            auto result = starter->childNodes[i]->findNode(name);
            if (result != nullptr) {
                return result;
            }
        }
        
        return nullptr;
    }
    
private:
    std::shared_ptr<Node> rootNode;
    Assimp::Importer importer;
};

}

#endif /* scene_hpp */
