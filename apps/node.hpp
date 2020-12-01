//
//  node.hpp
//  apps
//
//  Created by 杨丰 on 2020/12/1.
//

#ifndef node_hpp
#define node_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/scene.h>
#include <vector>
#include "shader.hpp"

namespace vox {
class Node {
public:
    virtual void draw(Shader &shader) = 0;
    
public:
    Node* parent = nullptr;
    std::vector<std::shared_ptr<Node>> childNodes;
    
    glm::mat4 localTransform = glm::mat4(1.0f);
    void setTransform(glm::mat4 transform) {
        this->localTransform = transform;
    }
    
    glm::mat4 modelTransform = glm::mat4(1.0f);
    bgfx::InstanceDataBuffer idb;
    bgfx::InstanceDataBuffer& getInstanceDataBuffer() {
        if (parent == nullptr) {
            return idb;
        }else {
            return parent->getInstanceDataBuffer();
        }
    }
};

}

#endif /* node_hpp */
