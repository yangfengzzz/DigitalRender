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
    std::shared_ptr<Node> findNode(std::string m_name) {
        //check childNode
        for (int i = 0; i < childNodes.size(); i++) {
            if (childNodes[i]->name == m_name) {
                return childNodes[i];
            }
        }
        
        //check subchild
        for (size_t i = 0; i < childNodes.size(); i++) {
            auto result = childNodes[i]->findNode(m_name);
            if (result != nullptr) {
                return result;
            }
        }
        
        return nullptr;
    }
    
public:
    std::string name = "";
    
    Node* parent = nullptr;
    std::vector<std::shared_ptr<Node>> childNodes;
    
    glm::mat4 localTransform = glm::mat4(1.0f);
    void setTransform(glm::mat4 transform) {
        this->localTransform = transform;
    }
    glm::mat4 modelTransform = glm::mat4(1.0f);
    
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
    
    Renderable* renderable = nullptr;
    void draw() {
        if (renderable != nullptr) {
            renderable->draw();
        }
        
        for (size_t i = 0; i < childNodes.size(); i++) {
            childNodes[i]->draw();
        }
    }
    
    //MARK:- Modify Methods
    void add(std::shared_ptr<Node> node) {
        childNodes.push_back(node);
    }
    
    void remove(std::shared_ptr<Node> node) {
        auto result = std::find(childNodes.begin(), childNodes.end(), node);
        if (result != childNodes.end()) {
            childNodes.erase(result);
        }
        
        for (int i = 0; i < node->childNodes.size(); i++) {
            childNodes.push_back(node->childNodes[i]);
        }
    }
};

}

#endif /* node_hpp */
