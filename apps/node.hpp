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
#include "renderable.hpp"

namespace vox {
class Node {
public:
    std::string name = "";
    
    Node* parent = nullptr;
    std::vector<std::shared_ptr<Node>> childNodes;
    
    glm::mat4 localTransform = glm::mat4(1.0f);
    void setTransform(glm::mat4 transform) {
        this->localTransform = transform;
    }
    glm::mat4 modelTransform = glm::mat4(1.0f);
    
    void update() {
        if (parent != nullptr) {
            modelTransform = parent->modelTransform * localTransform;
        } else {
            modelTransform = localTransform;
        }
    }
    
public:
    //MARK:- Instancing
    uint32_t instanceCount = 1;
    uint16_t instanceStride = 0;
    bool allocInstanceData(uint32_t count, uint16_t stride) {
        instanceCount = count;
        instanceStride = stride;
        
        if (instanceCount == bgfx::getAvailInstanceDataBuffer(instanceCount, instanceStride) )
        {
            bgfx::allocInstanceDataBuffer(&idb, instanceCount, instanceStride);
            return true;
        } else {
            return false;
        }
    }
    
    uint32_t getInstanceCount() {
        if (parent == nullptr) {
            return instanceCount;
        }else {
            return parent->getInstanceCount();
        }
    }
    
    bgfx::InstanceDataBuffer idb;
    bgfx::InstanceDataBuffer& getInstanceDataBuffer() {
        if (parent == nullptr) {
            return idb;
        }else {
            return parent->getInstanceDataBuffer();
        }
    }
    
public:
    //MARK:- Renderable
    Renderable* renderable = nullptr;
    void render() {
        update();
        if (renderable != nullptr) {
            renderable->draw();
        }
        
        for (size_t i = 0; i < childNodes.size(); i++) {
            childNodes[i]->render();
        }
    }
    
public:
    //MARK:- Modify Methods
    std::shared_ptr<Node> findNode(std::string m_name);
    
    void add(std::shared_ptr<Node> node);
    
    void remove(std::shared_ptr<Node> node);
};

}

#endif /* node_hpp */
