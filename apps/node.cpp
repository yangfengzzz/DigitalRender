//
//  node.cpp
//  apps
//
//  Created by 杨丰 on 2020/12/1.
//

#include "node.hpp"

namespace vox {
std::shared_ptr<Node> Node::findNode(std::string m_name) {
    //check childNode
    for (int i = 0; i < childNodes.size(); i++) {
        if (childNodes[i]->name == m_name) {
            return childNodes[i];
        }
    }
    
    //check subchild
    for (size_t i = 0; i < childNodes.size(); i++) {
        auto result = childNodes[i]->findNode(m_name);
        if (result != nullptr) {
            return result;
        }
    }
    
    return nullptr;
}

void Node::add(std::shared_ptr<Node> node) {
    childNodes.push_back(node);
}

void Node::remove(std::shared_ptr<Node> node) {
    auto result = std::find(childNodes.begin(), childNodes.end(), node);
    if (result != childNodes.end()) {
        childNodes.erase(result);
    }
    
    for (int i = 0; i < node->childNodes.size(); i++) {
        childNodes.push_back(node->childNodes[i]);
    }
}
}
