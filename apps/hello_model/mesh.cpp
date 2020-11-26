/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "common.h"
#include "camera.h"
#include "bgfx_utils.h"
#include "common/imgui/imgui.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace
{

class ExampleMesh : public entry::AppI
{
public:
    ExampleMesh(const char* _name, const char* _description, const char* _url)
    : entry::AppI(_name, _description, _url)
    {
    }
    
    void init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height) override
    {
        Args args(_argc, _argv);
        
        m_width  = _width;
        m_height = _height;
        m_debug  = BGFX_DEBUG_NONE;
        m_reset  = BGFX_RESET_VSYNC;
        
        bgfx::Init init;
        init.type     = args.m_type;
        init.vendorId = args.m_pciId;
        init.resolution.width  = m_width;
        init.resolution.height = m_height;
        init.resolution.reset  = m_reset;
        bgfx::init(init);
        
        // Enable debug text.
        bgfx::setDebug(m_debug);
        
        // Set view 0 clear state.
        bgfx::setViewClear(0
                           , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
                           , 0x303030ff
                           , 1.0f
                           , 0
                           );
        
        u_time = bgfx::createUniform("u_time", bgfx::UniformType::Vec4);
        
        // Create program from shaders.
        m_mesh = meshLoad("/Users/yangfeng/Desktop/DigitalRender/apps/hello_model/nanosuit.bin");
        m_state[0] = meshStateCreate();
        m_state[0]->m_state = 0;
        m_state[0]->m_program = m_progArm;
        m_state[0]->m_viewId  = 0;
        m_state[0]->m_numTextures = 3;
        m_progArm = loadProgram("../../../hello_model/vs_arm",
                                "../../../hello_model/fs_arm");
        m_state[0]->m_textures[0].m_flags = UINT32_MAX;
        m_state[0]->m_textures[0].m_stage = 0;
        m_state[0]->m_textures[0].m_sampler = s_arm_diff;
        m_state[0]->m_textures[0].m_texture = m_arm_diff;
        s_arm_diff  = bgfx::createUniform("s_arm_diff",  bgfx::UniformType::Sampler);
        m_arm_diff = loadTexture("/Users/yangfeng/Desktop/DigitalRender/apps/hello_model/arm_dif.png");
        m_state[0]->m_textures[1].m_flags = UINT32_MAX;
        m_state[0]->m_textures[1].m_stage = 0;
        m_state[0]->m_textures[1].m_sampler = s_arm_bump;
        m_state[0]->m_textures[1].m_texture = m_arm_bump;
        s_arm_bump  = bgfx::createUniform("s_arm_bump",  bgfx::UniformType::Sampler);
        m_arm_bump = loadTexture("/Users/yangfeng/Desktop/DigitalRender/apps/hello_model/arm_showroom_ddn.png");
        m_state[0]->m_textures[2].m_flags = UINT32_MAX;
        m_state[0]->m_textures[2].m_stage = 1;
        m_state[0]->m_textures[2].m_sampler = s_arm_spec;
        m_state[0]->m_textures[2].m_texture = m_arm_spec;
        s_arm_spec  = bgfx::createUniform("s_arm_spec",  bgfx::UniformType::Sampler);
        m_arm_spec = loadTexture("/Users/yangfeng/Desktop/DigitalRender/apps/hello_model/arm_showroom_spec.png");
        
        m_state[1] = meshStateCreate();
        m_state[1]->m_state = 0;
        m_state[1]->m_program = m_progBody;
        m_state[1]->m_viewId  = 0;
        m_state[1]->m_numTextures = 3;
        m_progBody = loadProgram("../../../hello_model/vs_body",
                                "../../../hello_model/fs_body");
        m_state[1]->m_textures[0].m_flags = UINT32_MAX;
        m_state[1]->m_textures[0].m_stage = 0;
        m_state[1]->m_textures[0].m_sampler = s_body_diff;
        m_state[1]->m_textures[0].m_texture = m_body_diff;
        s_body_diff  = bgfx::createUniform("s_body_diff",  bgfx::UniformType::Sampler);
        m_body_diff = loadTexture("/Users/yangfeng/Desktop/DigitalRender/apps/hello_model/body_dif.png");
        m_state[1]->m_textures[1].m_flags = UINT32_MAX;
        m_state[1]->m_textures[1].m_stage = 0;
        m_state[1]->m_textures[1].m_sampler = s_body_bump;
        m_state[1]->m_textures[1].m_texture = m_body_bump;
        s_body_bump  = bgfx::createUniform("s_body_bump",  bgfx::UniformType::Sampler);
        m_body_bump = loadTexture("/Users/yangfeng/Desktop/DigitalRender/apps/hello_model/body_showroom_ddn.png");
        m_state[1]->m_textures[2].m_flags = UINT32_MAX;
        m_state[1]->m_textures[2].m_stage = 1;
        m_state[1]->m_textures[2].m_sampler = s_body_spec;
        m_state[1]->m_textures[2].m_texture = m_body_spec;
        s_body_spec  = bgfx::createUniform("s_body_spec",  bgfx::UniformType::Sampler);
        m_body_spec = loadTexture("/Users/yangfeng/Desktop/DigitalRender/apps/hello_model/body_showroom_spec.png");
        
        m_state[2] = meshStateCreate();
        m_state[2]->m_state = 0;
        m_state[2]->m_program = m_progGlass;
        m_state[2]->m_viewId  = 0;
        m_state[2]->m_numTextures = 2;
        m_progGlass = loadProgram("../../../hello_model/vs_glass",
                                "../../../hello_model/fs_glass");
        m_state[2]->m_textures[0].m_flags = UINT32_MAX;
        m_state[2]->m_textures[0].m_stage = 0;
        m_state[2]->m_textures[0].m_sampler = s_glass_diff;
        m_state[2]->m_textures[0].m_texture = m_glass_diff;
        s_glass_diff  = bgfx::createUniform("s_glass_diff",  bgfx::UniformType::Sampler);
        m_glass_diff = loadTexture("/Users/yangfeng/Desktop/DigitalRender/apps/hello_model/glass_dif.png");
        m_state[2]->m_textures[1].m_flags = UINT32_MAX;
        m_state[2]->m_textures[1].m_stage = 0;
        m_state[2]->m_textures[1].m_sampler = s_glass_bump;
        m_state[2]->m_textures[1].m_texture = m_glass_bump;
        s_glass_bump  = bgfx::createUniform("s_glass_bump",  bgfx::UniformType::Sampler);
        m_glass_bump = loadTexture("/Users/yangfeng/Desktop/DigitalRender/apps/hello_model/glass_ddn.png");
        
        m_state[3] = meshStateCreate();
        m_state[3]->m_state = 0;
        m_state[3]->m_program = m_progHand;
        m_state[3]->m_viewId  = 0;
        m_state[3]->m_numTextures = 3;
        m_progHand = loadProgram("../../../hello_model/vs_hand",
                                "../../../hello_model/fs_hand");
        m_state[3]->m_textures[0].m_flags = UINT32_MAX;
        m_state[3]->m_textures[0].m_stage = 0;
        m_state[3]->m_textures[0].m_sampler = s_hand_diff;
        m_state[3]->m_textures[0].m_texture = m_hand_diff;
        s_hand_diff  = bgfx::createUniform("s_hand_diff",  bgfx::UniformType::Sampler);
        m_hand_diff = loadTexture("/Users/yangfeng/Desktop/DigitalRender/apps/hello_model/hand_dif.png");
        m_state[3]->m_textures[1].m_flags = UINT32_MAX;
        m_state[3]->m_textures[1].m_stage = 0;
        m_state[3]->m_textures[1].m_sampler = s_hand_bump;
        m_state[3]->m_textures[1].m_texture = m_hand_bump;
        s_hand_bump  = bgfx::createUniform("s_hand_bump",  bgfx::UniformType::Sampler);
        m_hand_bump = loadTexture("/Users/yangfeng/Desktop/DigitalRender/apps/hello_model/hand_showroom_ddn.png");
        m_state[3]->m_textures[2].m_flags = UINT32_MAX;
        m_state[3]->m_textures[2].m_stage = 1;
        m_state[3]->m_textures[2].m_sampler = s_hand_spec;
        m_state[3]->m_textures[2].m_texture = m_hand_spec;
        s_hand_spec  = bgfx::createUniform("s_hand_spec",  bgfx::UniformType::Sampler);
        m_hand_spec = loadTexture("/Users/yangfeng/Desktop/DigitalRender/apps/hello_model/hand_showroom_spec.png");
        
        m_state[4] = meshStateCreate();
        m_state[4]->m_state = 0;
        m_state[4]->m_program = m_progHelmet;
        m_state[4]->m_viewId  = 0;
        m_state[4]->m_numTextures = 3;
        m_progHelmet = loadProgram("../../../hello_model/vs_helmet",
                                "../../../hello_model/fs_helmet");
        m_state[4]->m_textures[0].m_flags = UINT32_MAX;
        m_state[4]->m_textures[0].m_stage = 0;
        m_state[4]->m_textures[0].m_sampler = s_helmet_diff;
        m_state[4]->m_textures[0].m_texture = m_helmet_diff;
        s_helmet_diff  = bgfx::createUniform("s_helmet_diff",  bgfx::UniformType::Sampler);
        m_helmet_diff = loadTexture("/Users/yangfeng/Desktop/DigitalRender/apps/hello_model/helmet_dif.png");
        m_state[4]->m_textures[1].m_flags = UINT32_MAX;
        m_state[4]->m_textures[1].m_stage = 0;
        m_state[4]->m_textures[1].m_sampler = s_helmet_bump;
        m_state[4]->m_textures[1].m_texture = m_helmet_bump;
        s_helmet_bump  = bgfx::createUniform("s_helmet_bump",  bgfx::UniformType::Sampler);
        m_helmet_bump = loadTexture("/Users/yangfeng/Desktop/DigitalRender/apps/hello_model/helmet_showroom_ddn.png");
        m_state[4]->m_textures[2].m_flags = UINT32_MAX;
        m_state[4]->m_textures[2].m_stage = 1;
        m_state[4]->m_textures[2].m_sampler = s_helmet_spec;
        m_state[4]->m_textures[2].m_texture = m_helmet_spec;
        s_helmet_spec  = bgfx::createUniform("s_helmet_spec",  bgfx::UniformType::Sampler);
        m_helmet_spec = loadTexture("/Users/yangfeng/Desktop/DigitalRender/apps/hello_model/helmet_showroom_spec.png");
        
        m_state[5] = meshStateCreate();
        m_state[5]->m_state = 0;
        m_state[5]->m_program = m_progLeg;
        m_state[5]->m_viewId  = 0;
        m_state[5]->m_numTextures = 3;
        m_progLeg = loadProgram("../../../hello_model/vs_leg",
                                "../../../hello_model/fs_leg");
        m_state[5]->m_textures[0].m_flags = UINT32_MAX;
        m_state[5]->m_textures[0].m_stage = 0;
        m_state[5]->m_textures[0].m_sampler = s_leg_diff;
        m_state[5]->m_textures[0].m_texture = m_leg_diff;
        s_leg_diff  = bgfx::createUniform("s_leg_diff",  bgfx::UniformType::Sampler);
        m_leg_diff = loadTexture("/Users/yangfeng/Desktop/DigitalRender/apps/hello_model/leg_dif.png");
        m_state[5]->m_textures[1].m_flags = UINT32_MAX;
        m_state[5]->m_textures[1].m_stage = 0;
        m_state[5]->m_textures[1].m_sampler = s_leg_bump;
        m_state[5]->m_textures[1].m_texture = m_leg_bump;
        s_leg_bump  = bgfx::createUniform("s_leg_bump",  bgfx::UniformType::Sampler);
        m_leg_bump = loadTexture("/Users/yangfeng/Desktop/DigitalRender/apps/hello_model/leg_showroom_ddn.png");
        m_state[5]->m_textures[2].m_flags = UINT32_MAX;
        m_state[5]->m_textures[2].m_stage = 1;
        m_state[5]->m_textures[2].m_sampler = s_leg_spec;
        m_state[5]->m_textures[2].m_texture = m_leg_spec;
        s_leg_spec  = bgfx::createUniform("s_leg_spec",  bgfx::UniformType::Sampler);
        m_leg_spec = loadTexture("/Users/yangfeng/Desktop/DigitalRender/apps/hello_model/leg_showroom_spec.png");
        
        // Set view and projection matrices.
        cameraCreate();
        cameraSetPosition({ 0.0f, 0.0f,  3.0f });
        cameraSetVerticalAngle(-0.35f);
        
        m_timeOffset = bx::getHPCounter();
        
        imguiCreate();
    }
    
    int shutdown() override
    {
        cameraDestroy();
        imguiDestroy();
        
        meshUnload(m_mesh);
        
        // Cleanup.
        bgfx::destroy(m_progArm);
        bgfx::destroy(m_progBody);
        bgfx::destroy(m_progGlass);
        bgfx::destroy(m_progHand);
        bgfx::destroy(m_progHelmet);
        bgfx::destroy(m_progLeg);
        
        bgfx::destroy(m_arm_spec);
        bgfx::destroy(s_arm_spec);
        bgfx::destroy(m_arm_diff);
        bgfx::destroy(s_arm_diff);
        bgfx::destroy(m_arm_bump);
        bgfx::destroy(s_arm_bump);
        
        bgfx::destroy(m_body_spec);
        bgfx::destroy(s_body_spec);
        bgfx::destroy(m_body_diff);
        bgfx::destroy(s_body_diff);
        bgfx::destroy(m_body_bump);
        bgfx::destroy(s_body_bump);
        
        bgfx::destroy(m_glass_diff);
        bgfx::destroy(s_glass_diff);
        bgfx::destroy(m_glass_bump);
        bgfx::destroy(s_glass_bump);
        
        bgfx::destroy(m_hand_spec);
        bgfx::destroy(s_hand_spec);
        bgfx::destroy(m_hand_diff);
        bgfx::destroy(s_hand_diff);
        bgfx::destroy(m_hand_bump);
        bgfx::destroy(s_hand_bump);
        
        bgfx::destroy(m_helmet_spec);
        bgfx::destroy(s_helmet_spec);
        bgfx::destroy(m_helmet_diff);
        bgfx::destroy(s_helmet_diff);
        bgfx::destroy(m_helmet_bump);
        bgfx::destroy(s_helmet_bump);
        
        bgfx::destroy(m_leg_spec);
        bgfx::destroy(s_leg_spec);
        bgfx::destroy(m_leg_diff);
        bgfx::destroy(s_leg_diff);
        bgfx::destroy(m_leg_bump);
        bgfx::destroy(s_leg_bump);
        
        bgfx::destroy(u_time);
        
        // Shutdown bgfx.
        bgfx::shutdown();
        
        return 0;
    }
    
    bool update() override
    {
        if (!entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState) )
        {
            imguiBeginFrame(m_mouseState.m_mx
                            ,  m_mouseState.m_my
                            , (m_mouseState.m_buttons[entry::MouseButton::Left  ] ? IMGUI_MBUT_LEFT   : 0)
                            | (m_mouseState.m_buttons[entry::MouseButton::Right ] ? IMGUI_MBUT_RIGHT  : 0)
                            | (m_mouseState.m_buttons[entry::MouseButton::Middle] ? IMGUI_MBUT_MIDDLE : 0)
                            ,  m_mouseState.m_mz
                            , uint16_t(m_width)
                            , uint16_t(m_height)
                            );
            
            showExampleDialog(this);
            
            imguiEndFrame();
            
            // Scale
            scroll_callback(double(m_mouseState.m_mz - m_lastScroll));
            m_lastScroll = m_mouseState.m_mz;
            
            // Time.
            int64_t now = bx::getHPCounter();
            static int64_t last = now;
            const int64_t frameTime = now - last;
            last = now;
            const double freq = double(bx::getHPFrequency() );
            const float deltaTime = float(frameTime/freq);
            
            // Set view and projection matrix for view 0.
            {
                // Update camera.
                float m_view[16];
                cameraUpdate(deltaTime, m_mouseState);
                cameraGetViewMtx(m_view);
                
                glm::mat4 projection = glm::mat4(1.0f);
                projection = glm::perspective(glm::radians(fov), float(m_width) / float(m_height), 0.1f, 100.0f);
                
                bgfx::setViewTransform(0, m_view, glm::value_ptr(projection));
                
                // Set view 0 default viewport.
                bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height) );
            }
            
            // This dummy draw call is here to make sure that view 0 is cleared
            // if no other draw calls are submitted to view 0.
            bgfx::touch(0);
            
            float time = (float)( (bx::getHPCounter()-m_timeOffset)/double(bx::getHPFrequency() ) );
            bgfx::setUniform(u_time, &time);
            
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));    // it's a bit too big for our scene, so scale it down
            model = glm::rotate(model, time*0.37f, glm::vec3(0.0f, 1.0f, 0.0f) );
            meshSubmit(m_mesh, m_state, 1, glm::value_ptr(model));
            
            // Advance to next frame. Rendering thread will be kicked to
            // process submitted rendering primitives.
            bgfx::frame();
            
            return true;
        }
        
        return false;
    }
    
    entry::MouseState m_mouseState;
    
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_debug;
    uint32_t m_reset;
    
    int64_t m_timeOffset;
    Mesh* m_mesh;
    MeshState* m_state[6];
    bgfx::ProgramHandle m_progArm;
    bgfx::ProgramHandle m_progBody;
    bgfx::ProgramHandle m_progGlass;
    bgfx::ProgramHandle m_progHand;
    bgfx::ProgramHandle m_progHelmet;
    bgfx::ProgramHandle m_progLeg;
    
    bgfx::UniformHandle u_time;
    
    bgfx::UniformHandle s_arm_diff;
    bgfx::TextureHandle m_arm_diff;
    bgfx::UniformHandle s_arm_bump;
    bgfx::TextureHandle m_arm_bump;
    bgfx::UniformHandle s_arm_spec;
    bgfx::TextureHandle m_arm_spec;
    
    bgfx::UniformHandle s_body_diff;
    bgfx::TextureHandle m_body_diff;
    bgfx::UniformHandle s_body_bump;
    bgfx::TextureHandle m_body_bump;
    bgfx::UniformHandle s_body_spec;
    bgfx::TextureHandle m_body_spec;
    
    bgfx::UniformHandle s_glass_diff;
    bgfx::TextureHandle m_glass_diff;
    bgfx::UniformHandle s_glass_bump;
    bgfx::TextureHandle m_glass_bump;
    
    bgfx::UniformHandle s_hand_diff;
    bgfx::TextureHandle m_hand_diff;
    bgfx::UniformHandle s_hand_bump;
    bgfx::TextureHandle m_hand_bump;
    bgfx::UniformHandle s_hand_spec;
    bgfx::TextureHandle m_hand_spec;
    
    bgfx::UniformHandle s_helmet_diff;
    bgfx::TextureHandle m_helmet_diff;
    bgfx::UniformHandle s_helmet_bump;
    bgfx::TextureHandle m_helmet_bump;
    bgfx::UniformHandle s_helmet_spec;
    bgfx::TextureHandle m_helmet_spec;
    
    bgfx::UniformHandle s_leg_diff;
    bgfx::TextureHandle m_leg_diff;
    bgfx::UniformHandle s_leg_bump;
    bgfx::TextureHandle m_leg_bump;
    bgfx::UniformHandle s_leg_spec;
    bgfx::TextureHandle m_leg_spec;
    
    int32_t m_lastScroll = 0;
    float fov   =  45.0f;
    void scroll_callback(double yoffset)
    {
        if(fov >= 1.0f && fov <= 45.0f)
            fov -= yoffset;
        if(fov <= 1.0f)
            fov = 1.0f;
        if(fov >= 45.0f)
            fov = 45.0f;
    }
};

} // namespace

ENTRY_IMPLEMENT_MAIN(
                     ExampleMesh
                     , "04-mesh"
                     , "Loading meshes."
                     , "https://bkaradzic.github.io/bgfx/examples.html#mesh"
                     );
