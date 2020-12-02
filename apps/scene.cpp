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
std::shared_ptr<Model> Scene::loadAssimp(std::string const &path, const Shader& shader) {
    const aiScene* scene = importer.ReadFile(path,
                                             aiProcess_Triangulate |
                                             aiProcess_GenSmoothNormals |
                                             aiProcess_FlipUVs |
                                             aiProcess_CalcTangentSpace);
    // check for errors
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return nullptr;
    }
    // retrieve the directory path of the filepath
    std::string directory = path.substr(0, path.find_last_of('/'));
    
    return std::make_shared<Model>(directory, scene->mRootNode,
                                   scene, rootNode.get(), shader);
}

std::shared_ptr<Node> Scene::findNode(std::string name,
                                      std::shared_ptr<Node> starter) {
    if (starter == nullptr) {
        starter = rootNode;
    }
    
    //check self
    if (starter->name == name) {
        return starter;
    }
    
    //check childNode
    for (int i = 0; i < starter->childNodes.size(); i++) {
        if (starter->childNodes[i]->name == name) {
            return starter->childNodes[i];
        }
    }
    
    //check subchild
    for (int i = 0; i < starter->childNodes.size(); i++) {
        auto result = starter->childNodes[i]->findNode(name);
        if (result != nullptr) {
            return result;
        }
    }
    
    return nullptr;
}

}
