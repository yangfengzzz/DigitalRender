//
//  transform.cpp
//  apps
//
//  Created by 杨丰 on 2020/12/2.
//

#include "transform.hpp"

namespace vox {
glm::mat4 Transform::modelMatrix() {
    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    model = glm::rotate(model, rotation.x, glm::vec3(1.0, 0.0, 0.0));
    model = glm::rotate(model, rotation.y, glm::vec3(0.0, 1.0, 0.0));
    model = glm::rotate(model, rotation.z, glm::vec3(0.0, 0.0, 1.0));
    return model;
}

glm::mat3 Transform::normalMatrix() {
    return glm::mat3(modelMatrix());
}
}
