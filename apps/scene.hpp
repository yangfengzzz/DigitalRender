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
#include "renderable.hpp"
#include "shader.hpp"

namespace vox {
class Scene {
public:    
    void allocRoot() {
        rootNode = std::make_shared<Node>();
        rootNode->name = "RootNode";
        rootNode->allocInstanceData(1);
        rootNode->updateBuffer(0, Transform());
    }
    
    void loadAssimp(std::string const &path,
                    const Shader& shader);
    
    // draws the model, and thus all its meshes
    void draw()
    {
        rootNode->render();
    }
    
    void setTransform(glm::mat4 transform) {
        rootNode->localTransform = transform;
        
    }
    
    std::shared_ptr<Node> getRoot(){
        return rootNode;
    }
    
    std::shared_ptr<Node> findNode(std::string name,
                                   std::shared_ptr<Node> starter = nullptr);
    
private:
    std::shared_ptr<Node> rootNode;
    Assimp::Importer importer;
};

}

#endif /* scene_hpp */
