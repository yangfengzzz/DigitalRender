//
//  camera.hpp
//  apps
//
//  Created by 杨丰 on 2020/12/17.
//

#ifndef camera_hpp
#define camera_hpp

#include "common.h"
#include "camera.h"
#include "node.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace vox {
class Camera : public Node {
public:
    Camera() {}
    
    ~Camera() {
        cameraDestroy();
    }
    
public:
    void create() {
        cameraCreate();
    }
    
    virtual void zoom(float delta) {}
    virtual void rotate(glm::vec2 delta) {}
    
public:
    float fov = 60.0f;
    float fovRadians() {
        return glm::radians(fov);
    }
    
    float aspect = 1;
    float near = 0.001;
    float far = 100;
    
    glm::mat4 projectionMatrix() {
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(fovRadians(), aspect, near, far);
        return projection;
    }
    
    glm::mat4 viewMatrix() {
        glm::mat4 view = glm::mat4(1.0f);
        cameraGetViewMtx(glm::value_ptr(view));
        return view;
    }
};

//MARK:- ArcballCamera
/// allows to rotate the scene and zoom into it
class ArcballCamera: public Camera {
public:
    float minDistance = 0.5;
    float maxDistance = 10;
};
}

#endif /* camera_hpp */
