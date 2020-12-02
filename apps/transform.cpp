//
//  transform.cpp
//  apps
//
//  Created by 杨丰 on 2020/12/2.
//

#include "transform.hpp"

namespace vox {
glm::mat4 Transform::modelMatrix() const {
    glm::mat4 identity = glm::mat4(1.0);
    glm::mat4 translate_mat = glm::translate(identity, position);
    glm::mat4 rotation_mat = glm::mat4_cast(glm::quat(eulerAngle));
    glm::mat4 scale_mat = glm::scale(identity, scale);
    return translate_mat * rotation_mat * scale_mat;
}

glm::mat3 Transform::normalMatrix() const {
    return glm::mat3(modelMatrix());
}
}
