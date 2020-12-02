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
#include <glm/gtc/quaternion.hpp>
#include <assimp/scene.h>
#include <vector>
#include "shader.hpp"
#include "renderable.hpp"

namespace vox {
class Node {
public:
    //MARK:- Node Transform
    std::string name = "untitled";
    glm::vec3 position = glm::vec3(0.0);
    glm::quat quaternion = glm::quat(0.0, 0.0, 0.0, 0.0);
    glm::vec3 scale = glm::vec3(1.0f);
    void setEulerAngle(glm::vec3 eulerAngle) {
        quaternion = glm::quat(eulerAngle);
    }
    
    /// User setting transform
    glm::mat4 modelMatrix() {
        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(model, position);
        model = glm::scale(model, scale);
        model = glm::mat4_cast(quaternion) * model;
        return model;
    }
    
    /// Accumulate user setting trasform
    glm::mat4 worldTransform() {
        if (parent != nullptr) {
            return parent->worldTransform() * modelMatrix();
        }
        return modelMatrix();
    }
    
    /// model internal transform
    glm::mat4 localTransform = glm::mat4(1.0f);
    void setTransform(glm::mat4 transform) {
        this->localTransform = transform;
    }
    
    Node* parent = nullptr;
    std::vector<std::shared_ptr<Node>> childNodes;
    
public:
    //MARK:- Instancing
    bool isInstanceParent = false;
    uint32_t instanceCount = 1;
    uint16_t instanceStride = 0;
    bool allocInstanceData(uint32_t count, uint16_t stride) {
        instanceCount = count;
        instanceStride = stride;
        
        if (instanceCount == bgfx::getAvailInstanceDataBuffer(instanceCount, instanceStride) )
        {
            bgfx::allocInstanceDataBuffer(&idb, instanceCount, instanceStride);
            isInstanceParent = true;
            return true;
        } else {
            return false;
        }
    }
    
    uint32_t getInstanceCount() {
        if (parent == nullptr || isInstanceParent) {
            return instanceCount;
        }else {
            return parent->getInstanceCount();
        }
    }
    
    bgfx::InstanceDataBuffer idb;
    bgfx::InstanceDataBuffer& getInstanceDataBuffer() {
        if (parent == nullptr || isInstanceParent) {
            return idb;
        }else {
            return parent->getInstanceDataBuffer();
        }
    }
    
public:
    //MARK:- Renderable
    Renderable* renderable = nullptr;
    void render() {
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
