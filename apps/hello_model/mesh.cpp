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
        m_program = loadProgram("vs_mesh", "fs_mesh");
        
        m_mesh = meshLoad("/Users/yangfeng/Desktop/DigitalRender/apps/hello_model/nanosuit.bin");
        
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
        bgfx::destroy(m_program);
        
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
            meshSubmit(m_mesh, 0, m_program, glm::value_ptr(model));
            
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
    bgfx::ProgramHandle m_program;
    bgfx::UniformHandle u_time;
    
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
