//
//  transform.hpp
//  apps
//
//  Created by 杨丰 on 2020/12/2.
//

#ifndef transform_hpp
#define transform_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace vox {
struct Transform {
    glm::vec3 position = glm::vec3(0.0);
    glm::vec3 rotation = glm::vec3(0.0);
    glm::vec3 scale = glm::vec3(1.0);
    
public:
    glm::mat4 modelMatrix();
    glm::mat3 normalMatrix();
};

}

#endif /* transform_hpp */
