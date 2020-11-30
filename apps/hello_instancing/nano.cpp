/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "common.h"
#include "camera.h"
#include "model.hpp"
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
        
        m_model.load("/Users/yangfeng/Desktop/DigitalRender/apps/hello_instancing/nanosuit.obj");
        m_shader.addShader(3, "../../../hello_instancing/vs_model",
                           "../../../hello_instancing/fs_two");
        m_shader.addShader(2, "../../../hello_instancing/vs_model",
                           "../../../hello_instancing/fs_one");
        
        // Set view and projection matrices.
        cameraCreate();
        cameraSetPosition({ 0.0f, 0.0f,  3.0f });
        cameraSetVerticalAngle(-0.35f);
        
        viewPos = bgfx::createUniform("viewPos", bgfx::UniformType::Vec4);
        spotLight = bgfx::createUniform("spotLight", bgfx::UniformType::Vec4, 7);
        dirLight = bgfx::createUniform("dirLight", bgfx::UniformType::Vec4, 4);
        
        m_timeOffset = bx::getHPCounter();
        
        imguiCreate();
    }
    
    int shutdown() override
    {
        cameraDestroy();
        imguiDestroy();
        
        // Cleanup.
        bgfx::destroy(viewPos);
        bgfx::destroy(spotLight);
        bgfx::destroy(dirLight);
        
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
                projection = glm::perspective(glm::radians(fov), float(m_width) / float(m_height), 0.1f, 1000.0f);
                
                bgfx::setViewTransform(0, m_view, glm::value_ptr(projection));
                
                // Set view 0 default viewport.
                bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height) );
            }
            
            bx::Vec3 viewPos_tmp = cameraGetPosition();
            glm::vec4 u_viewPos = glm::vec4(viewPos_tmp.x, viewPos_tmp.y, viewPos_tmp.z, 0);
            bgfx::setUniform(viewPos, &u_viewPos);
            
            float dirLight_data[4][4];
            {
                //dirLight.direction
                dirLight_data[0][0] = 0.2f;
                dirLight_data[0][1] = 0.0f;
                dirLight_data[0][2] = -1.0f;
                dirLight_data[0][3] = 0.0f;
                
                //dirLight.ambient
                dirLight_data[1][0] = 0.5f;
                dirLight_data[1][1] = 0.5f;
                dirLight_data[1][2] = 0.5f;
                dirLight_data[1][3] = 0.0f;
                
                //dirLight.diffuse
                dirLight_data[2][0] = 0.4f;
                dirLight_data[2][1] = 0.4f;
                dirLight_data[2][2] = 0.4f;
                dirLight_data[2][3] = 0.4f;
                
                //dirLight.specular
                dirLight_data[3][0] = 0.5f;
                dirLight_data[3][1] = 0.5f;
                dirLight_data[3][2] = 0.5f;
                dirLight_data[3][3] = 0.0f;
                
                bgfx::setUniform(dirLight, &dirLight_data, 4);
            }
            
            bx::Vec3 camera_front = cameraGetAt();
            float spotLight_data[7][4];
            {
                //spotLight.position
                spotLight_data[0][0] = viewPos_tmp.x;
                spotLight_data[0][1] = viewPos_tmp.y;
                spotLight_data[0][2] = viewPos_tmp.z;
                spotLight_data[0][3] = 0.0f;
                
                //spotLight.direction
                spotLight_data[1][0] = viewPos_tmp.x - camera_front.x;
                spotLight_data[1][1] = viewPos_tmp.y - camera_front.y;
                spotLight_data[1][2] = viewPos_tmp.z - camera_front.z;
                spotLight_data[1][3] = 0.0f;
                
                //spotLight.cutOff, outerCutOff
                spotLight_data[2][0] = glm::cos(glm::radians(12.5f));
                spotLight_data[2][1] = glm::cos(glm::radians(15.0f));
                spotLight_data[2][2] = 0.0f;
                spotLight_data[2][3] = 0.0f;
                
                //spotLight.paras(constant, linear, quadratic)
                spotLight_data[3][0] = 1.0f;
                spotLight_data[3][1] = 0.09f;
                spotLight_data[3][2] = 0.032f;
                spotLight_data[3][3] = 0.0f;
                
                //spotLight.ambient
                spotLight_data[4][0] = 0.0f;
                spotLight_data[4][1] = 0.0f;
                spotLight_data[4][2] = 0.0f;
                spotLight_data[4][3] = 0.0f;
                
                //spotLight.diffuse
                spotLight_data[5][0] = 1.0f;
                spotLight_data[5][1] = 1.0f;
                spotLight_data[5][2] = 1.0f;
                spotLight_data[5][3] = 0.0f;
                
                //spotLight.specular
                spotLight_data[6][0] = 1.0f;
                spotLight_data[6][1] = 1.0f;
                spotLight_data[6][2] = 1.0f;
                spotLight_data[6][3] = 0.0f;
                
                bgfx::setUniform(spotLight, &spotLight_data, 7);
            }
            
            // This dummy draw call is here to make sure that view 0 is cleared
            // if no other draw calls are submitted to view 0.
            bgfx::touch(0);
            
            float time = (float)( (bx::getHPCounter()-m_timeOffset)/double(bx::getHPFrequency() ) );
            bgfx::setUniform(u_time, &time);
            
            // 64 bytes for 4x4 matrix.
            const uint16_t instanceStride = 64;
            // 11x11 models
            const uint32_t numInstances   = 121;
            if (numInstances == bgfx::getAvailInstanceDataBuffer(numInstances, instanceStride) )
            {
                bgfx::allocInstanceDataBuffer(&m_model.idb, numInstances, instanceStride);
                
                uint8_t* data = m_model.idb.data;
                
                // Write instance data for 11x11 cubes.
                for (uint32_t yy = 0; yy < 11; ++yy)
                {
                    for (uint32_t xx = 0; xx < 11; ++xx)
                    {
                        float* mtx = (float*)data;
                        bx::mtxRotateY(mtx, time*0.37f);
                        mtx[12] = -15.0f + float(xx)*10.0f;
                        mtx[13] = 0.0f;
                        mtx[14] = -15.0f + float(yy)*10.0f;
                        
                        data += instanceStride;
                    }
                }
                m_model.draw(m_shader);
            }
            
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
    vox::Model m_model;
    vox::Shader m_shader;
    
    int32_t m_lastScroll = 0;
    float fov   =  60.0f;
    void scroll_callback(double yoffset)
    {
        if(fov >= 1.0f && fov <= 45.0f)
            fov -= yoffset;
        if(fov <= 1.0f)
            fov = 1.0f;
        if(fov >= 45.0f)
            fov = 45.0f;
    }
    
    bgfx::UniformHandle u_time;
    bgfx::UniformHandle viewPos;
    bgfx::UniformHandle spotLight;
    bgfx::UniformHandle dirLight;
};

} // namespace

ENTRY_IMPLEMENT_MAIN(
                     ExampleMesh
                     , "04-mesh"
                     , "Loading meshes."
                     , "https://bkaradzic.github.io/bgfx/examples.html#mesh"
                     );
