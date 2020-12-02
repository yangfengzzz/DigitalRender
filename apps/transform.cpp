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
    glm::quat rotation(eulerAngle);
    return glm::mat4_cast(rotation) * model;
}

glm::mat3 Transform::normalMatrix() {
    return glm::mat3(modelMatrix());
}
}
