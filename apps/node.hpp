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
#include "transform.hpp"

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
        glm::mat4 identity = glm::mat4(1.0);
        glm::mat4 translate_mat = glm::translate(identity, position);
        glm::mat4 rotation_mat = glm::mat4_cast(quaternion);
        glm::mat4 scale_mat = glm::scale(identity, scale);
        return translate_mat * rotation_mat * scale_mat;
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
    /// 64 bytes for 4x4 model matrix.
    static constexpr uint16_t instanceStride = 4*(4*4);
    uint32_t instanceCount = 0;
    std::vector<Transform> transforms;
    void setInstanceCount(uint32_t count) {
        instanceCount = count;
        transforms = std::vector<Transform>(count);
    }
    
    bgfx::InstanceDataBuffer idb;
    bool isInstanceParent = false;
    bool allocInstanceData() {
        if (instanceCount == bgfx::getAvailInstanceDataBuffer(instanceCount, instanceStride) )
        {
            bgfx::allocInstanceDataBuffer(&idb, instanceCount, instanceStride);
            isInstanceParent = true;
            return true;
        } else {
            return false;
        }
    }
    
    void updateInstanceBuffer() {
        allocInstanceData();
        
        uint8_t* data = idb.data;
        for (size_t t = 0; t < instanceCount; t++) {
            float* mtx = (float*)data;
            glm::mat4 model = transforms[t].modelMatrix();
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    *mtx = model[i][j];
                    mtx++;
                }
            }
            data += instanceStride;
        }
    }
    
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
