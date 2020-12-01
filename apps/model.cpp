//
//  model.cpp
//  apps
//
//  Created by 杨丰 on 2020/11/30.
//

#include "model.hpp"
#include <bx/pixelformat.h>
#include <bx/commandline.h>
#include <bx/endian.h>
#include <bx/math.h>
#include <bx/readerwriter.h>
#include <bx/string.h>
#include <bimg/decode.h>
#include "common/entry/entry.h"

namespace vox {
void Model::processNode() {
    meshes.resize(node->mNumMeshes);
    // process each mesh located at the current node
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene.
        // the scene contains all the data,
        // node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        meshes[i].load(mesh, material, this);
    }
}

}
