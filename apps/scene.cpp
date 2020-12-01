//
//  scene.cpp
//  apps
//
//  Created by 杨丰 on 2020/12/1.
//

#include "scene.hpp"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "model.hpp"

namespace vox {
void Scene::loadAssimp(std::string const &path, const Shader& shader) {
    const aiScene* scene = importer.ReadFile(path,
                                             aiProcess_Triangulate |
                                             aiProcess_GenSmoothNormals |
                                             aiProcess_FlipUVs |
                                             aiProcess_CalcTangentSpace);
    // check for errors
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    // retrieve the directory path of the filepath
    std::string directory = path.substr(0, path.find_last_of('/'));
    
    rootNode = std::make_shared<Model>(directory, scene->mRootNode, scene, nullptr, shader);
}

}
