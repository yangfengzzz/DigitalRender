//
//  texture.hpp
//  apps
//
//  Created by 杨丰 on 2020/12/1.
//

#ifndef texture_hpp
#define texture_hpp

#include <bgfx/bgfx.h>
#include <string>

namespace vox {
struct Texture {
    bgfx::TextureHandle m_texture;
    std::string type;
    std::string path;
};
}

#endif /* texture_hpp */
