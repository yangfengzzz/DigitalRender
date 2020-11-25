//
//  hello_light.cpp
//  apps
//
//  Created by 杨丰 on 2020/11/24.
//

#include "common.h"
#include "bgfx_utils.h"
#include "camera.h"
#include "common/imgui/imgui.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace
{

struct PosColorVertex
{
    float m_x;
    float m_y;
    float m_z;
    uint32_t m_normal;
    int16_t m_u;
    int16_t m_v;
    
    static void init()
    {
        ms_layout
        .begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal,   4, bgfx::AttribType::Uint8, true, true)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Int16, true, true)
        .end();
    };
    
    static bgfx::VertexLayout ms_layout;
};

bgfx::VertexLayout PosColorVertex::ms_layout;

static PosColorVertex s_cubeVertices[] =
{
    {-0.5f, -0.5f, -0.5f, encodeNormalRgba8( 0.0f,  0.0f,  -1.0f),  0, 0},
    { 0.5f, -0.5f, -0.5f, encodeNormalRgba8( 0.0f,  0.0f,  -1.0f),  0x7fff, 0},
    { 0.5f,  0.5f, -0.5f, encodeNormalRgba8( 0.0f,  0.0f,  -1.0f),  0x7fff, 0x7fff},
    { 0.5f,  0.5f, -0.5f, encodeNormalRgba8( 0.0f,  0.0f,  -1.0f),  0x7fff, 0x7fff},
    {-0.5f,  0.5f, -0.5f, encodeNormalRgba8( 0.0f,  0.0f,  -1.0f),  0, 0x7fff},
    {-0.5f, -0.5f, -0.5f, encodeNormalRgba8( 0.0f,  0.0f,  -1.0f),  0, 0},
    
    {-0.5f, -0.5f,  0.5f, encodeNormalRgba8( 0.0f,  0.0f,  1.0f),  0, 0},
    { 0.5f, -0.5f,  0.5f, encodeNormalRgba8( 0.0f,  0.0f,  1.0f),  0x7fff, 0},
    { 0.5f,  0.5f,  0.5f, encodeNormalRgba8( 0.0f,  0.0f,  1.0f),  0x7fff, 0x7fff},
    { 0.5f,  0.5f,  0.5f, encodeNormalRgba8( 0.0f,  0.0f,  1.0f),  0x7fff, 0x7fff},
    {-0.5f,  0.5f,  0.5f, encodeNormalRgba8( 0.0f,  0.0f,  1.0f),  0, 0x7fff},
    {-0.5f, -0.5f,  0.5f, encodeNormalRgba8( 0.0f,  0.0f,  1.0f),  0, 0},
    
    {-0.5f,  0.5f,  0.5f, encodeNormalRgba8( -1.0f,  0.0f,  0.0f),  0x7fff, 0},
    { -0.5f,  0.5f, -0.5f, encodeNormalRgba8( -1.0f,  0.0f,  0.0f),  0x7fff, 0x7fff},
    { -0.5f, -0.5f, -0.5f, encodeNormalRgba8( -1.0f,  0.0f,  0.0f),  0, 0x7fff},
    { -0.5f, -0.5f, -0.5f, encodeNormalRgba8( -1.0f,  0.0f,  0.0f),  0, 0x7fff},
    { -0.5f, -0.5f,  0.5f, encodeNormalRgba8( -1.0f,  0.0f,  0.0f),  0, 0},
    { -0.5f,  0.5f,  0.5f, encodeNormalRgba8( -1.0f,  0.0f,  0.0f),  0x7fff, 0},
    
    {0.5f,  0.5f,  0.5f, encodeNormalRgba8( 1.0f,  0.0f,  0.0f),  0x7fff, 0},
    {0.5f,  0.5f, -0.5f, encodeNormalRgba8( 1.0f,  0.0f,  0.0f),  0x7fff, 0x7fff},
    {0.5f, -0.5f, -0.5f, encodeNormalRgba8( 1.0f,  0.0f,  0.0f),  0, 0x7fff},
    {0.5f, -0.5f, -0.5f, encodeNormalRgba8( 1.0f,  0.0f,  0.0f),  0, 0x7fff},
    {0.5f, -0.5f,  0.5f, encodeNormalRgba8( 1.0f,  0.0f,  0.0f),  0, 0},
    {0.5f,  0.5f,  0.5f, encodeNormalRgba8( 1.0f,  0.0f,  0.0f),  0x7fff, 0},
    
    {-0.5f, -0.5f, -0.5f, encodeNormalRgba8( 0.0f,  -1.0f,  0.0f),  0, 0x7fff},
    {0.5f, -0.5f, -0.5f, encodeNormalRgba8( 0.0f,  -1.0f,  0.0f),  0x7fff, 0x7fff},
    {0.5f, -0.5f,  0.5f, encodeNormalRgba8( 0.0f,  -1.0f,  0.0f),  0x7fff, 0},
    {0.5f, -0.5f,  0.5f, encodeNormalRgba8( 0.0f,  -1.0f,  0.0f),  0x7fff, 0},
    {-0.5f, -0.5f,  0.5f, encodeNormalRgba8( 0.0f,  -1.0f,  0.0f),  0, 0},
    {-0.5f, -0.5f, -0.5f, encodeNormalRgba8( 0.0f,  -1.0f,  0.0f),  0, 0x7fff},
    
    {-0.5f,  0.5f, -0.5f, encodeNormalRgba8( 0.0f,  1.0f,  0.0f),  0, 0x7fff},
    {0.5f,  0.5f, -0.5f, encodeNormalRgba8( 0.0f,  1.0f,  0.0f),  0x7fff, 0x7fff},
    {0.5f,  0.5f,  0.5f, encodeNormalRgba8( 0.0f,  1.0f,  0.0f),  0x7fff, 0},
    {0.5f,  0.5f,  0.5f, encodeNormalRgba8( 0.0f,  1.0f,  0.0f),  0x7fff, 0},
    {-0.5f,  0.5f,  0.5f, encodeNormalRgba8( 0.0f,  1.0f,  0.0f),  0, 0},
    {-0.5f,  0.5f, -0.5f, encodeNormalRgba8( 0.0f,  1.0f,  0.0f),  0, 0x7fff}
};

static const uint16_t s_cubeTriList[] =
{
    2,  1,  0,
    5,  4,  3,
    
    6,  7,  8,
    9,  10,  11,
    
    12,  13,  14,
    15,  16,  17,
    
    20,  19,  18,
    23,  22,  21,
    
    24,  25,  26,
    27,  28,  29,
    
    32,  31,  30,
    35,  34,  33,
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};

static const char* s_ptNames[]
{
    "Triangle List",
};

static const uint64_t s_ptState[]
{
    UINT64_C(0),
};
BX_STATIC_ASSERT(BX_COUNTOF(s_ptState) == BX_COUNTOF(s_ptNames) );

class ExampleLight : public entry::AppI
{
public:
    ExampleLight(const char* _name, const char* _description, const char* _url)
    : entry::AppI(_name, _description, _url)
    , m_pt(0)
    , m_r(true)
    , m_g(true)
    , m_b(true)
    , m_a(true)
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
        
        // Create vertex stream declaration.
        PosColorVertex::init();
        
        // Create static vertex buffer.
        m_vbh = bgfx::createVertexBuffer(
                                         // Static data can be passed with bgfx::makeRef
                                         bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices) )
                                         , PosColorVertex::ms_layout
                                         );
        
        // Create static index buffer for triangle list rendering.
        m_ibh[0] = bgfx::createIndexBuffer(
                                           // Static data can be passed with bgfx::makeRef
                                           bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList) )
                                           );
        
        viewPos = bgfx::createUniform("viewPos", bgfx::UniformType::Vec4);
        normal_matrix = bgfx::createUniform("normal_matrix", bgfx::UniformType::Mat4);
        
        material = bgfx::createUniform("material", bgfx::UniformType::Vec4, 2);
        dirLight = bgfx::createUniform("dirLight", bgfx::UniformType::Vec4, 4);
        pointLights[0] = bgfx::createUniform("pointLights0", bgfx::UniformType::Vec4, 7);
        pointLights[1] = bgfx::createUniform("pointLights1", bgfx::UniformType::Vec4, 7);
        pointLights[2] = bgfx::createUniform("pointLights2", bgfx::UniformType::Vec4, 7);
        pointLights[3] = bgfx::createUniform("pointLights3", bgfx::UniformType::Vec4, 7);
        spotLight = bgfx::createUniform("spotLight", bgfx::UniformType::Vec4, 10);
        
        // Create program from shaders.
        m_program_box = loadProgram("../../../hello_light/vs_light",
                                    "../../../hello_light/fs_box");
        m_program_light = loadProgram("../../../hello_light/vs_light",
                                      "../../../hello_light/fs_light");
        
        // Create texture sampler uniforms.
        s_diffuse  = bgfx::createUniform("s_diffuse",  bgfx::UniformType::Sampler);
        s_specular  = bgfx::createUniform("s_specular",  bgfx::UniformType::Sampler);
        
        // Load diffuse texture.
        m_diffuse = loadTexture("/Users/yangfeng/Desktop/DigitalRender/apps/hello_light/container2.png");
        m_specular = loadTexture("/Users/yangfeng/Desktop/DigitalRender/apps/hello_light/container2_specular.png");
        
        // Set view and projection matrices.
        cameraCreate();
        cameraSetPosition({ 0.0f, 0.0f,  3.0f });
        cameraSetVerticalAngle(-0.35f);
        
        m_timeOffset = bx::getHPCounter();
        
        imguiCreate();
    }
    
    virtual int shutdown() override
    {
        cameraDestroy();
        imguiDestroy();
        
        // Cleanup.
        for (uint32_t ii = 0; ii < BX_COUNTOF(m_ibh); ++ii)
        {
            bgfx::destroy(m_ibh[ii]);
        }
        
        bgfx::destroy(m_vbh);
        bgfx::destroy(m_program_box);
        bgfx::destroy(m_program_light);
        
        bgfx::destroy(viewPos);
        bgfx::destroy(normal_matrix);
        
        bgfx::destroy(material);
        bgfx::destroy(dirLight);
        bgfx::destroy(pointLights[0]);
        bgfx::destroy(pointLights[1]);
        bgfx::destroy(pointLights[2]);
        bgfx::destroy(pointLights[3]);
        bgfx::destroy(spotLight);
        
        bgfx::destroy(m_diffuse);
        bgfx::destroy(m_specular);
        bgfx::destroy(s_diffuse);
        bgfx::destroy(s_specular);
        
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
            
            ImGui::SetNextWindowPos(
                                    ImVec2(m_width - m_width / 5.0f - 10.0f, 10.0f)
                                    , ImGuiCond_FirstUseEver
                                    );
            ImGui::SetNextWindowSize(
                                     ImVec2(m_width / 5.0f, m_height / 3.5f)
                                     , ImGuiCond_FirstUseEver
                                     );
            ImGui::Begin("Settings"
                         , NULL
                         , 0
                         );
            
            ImGui::Checkbox("Write R", &m_r);
            ImGui::Checkbox("Write G", &m_g);
            ImGui::Checkbox("Write B", &m_b);
            ImGui::Checkbox("Write A", &m_a);
            
            ImGui::Text("Primitive topology:");
            ImGui::Combo("", (int*)&m_pt, s_ptNames, BX_COUNTOF(s_ptNames) );
            
            ImGui::End();
            
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
            
            bgfx::IndexBufferHandle ibh = m_ibh[m_pt];
            uint64_t state = 0
            | (m_r ? BGFX_STATE_WRITE_R : 0)
            | (m_g ? BGFX_STATE_WRITE_G : 0)
            | (m_b ? BGFX_STATE_WRITE_B : 0)
            | (m_a ? BGFX_STATE_WRITE_A : 0)
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | BGFX_STATE_CULL_CW
            | BGFX_STATE_MSAA
            | s_ptState[m_pt]
            ;
            
            //Main Box
            {
                bx::Vec3 viewPos_tmp = cameraGetPosition();
                glm::vec4 u_viewPos = glm::vec4(viewPos_tmp.x, viewPos_tmp.y, viewPos_tmp.z, 0);
                bgfx::setUniform(viewPos, &u_viewPos);
                
                //Materials
                {
                    // Bind textures.
                    bgfx::setTexture(0, s_diffuse,  m_diffuse);
                    bgfx::setTexture(1, s_specular,  m_specular);
                    
                    float material_data[2][4];
                    //material.ambient
                    material_data[0][0] = 1.0f;
                    material_data[0][1] = 0.5f;
                    material_data[0][2] = 0.31f;
                    material_data[0][3] = 0.0f;
                    
                    //material.shininess
                    material_data[1][0] = 32.0f;
                    material_data[1][1] = 0.0f;
                    material_data[1][2] = 0.0f;
                    material_data[1][3] = 0.0f;
                    bgfx::setUniform(material, material_data, 2);
                }
                
                //Lights
                {
                    float dirLight_data[4][4];
                    {
                        //dirLight.direction
                        dirLight_data[0][0] = -0.2f;
                        dirLight_data[0][1] = -1.0f;
                        dirLight_data[0][2] = -0.3f;
                        dirLight_data[0][3] = 0.0f;
                        
                        //dirLight.ambient
                        dirLight_data[1][0] = 0.05f;
                        dirLight_data[1][1] = 0.05f;
                        dirLight_data[1][2] = 0.05f;
                        dirLight_data[1][3] = 0.0f;
                        
                        //dirLight.diffuse
                        dirLight_data[2][0] = 0.4f;
                        dirLight_data[2][1] = 0.4f;
                        dirLight_data[2][2] = 0.4f;
                        dirLight_data[2][3] = 0.0f;
                        
                        //dirLight.specular
                        dirLight_data[3][0] = 0.5f;
                        dirLight_data[3][1] = 0.5f;
                        dirLight_data[3][2] = 0.5f;
                        dirLight_data[3][3] = 0.0f;
                        
                        bgfx::setUniform(dirLight, &dirLight_data, 4);
                    }
                    
                    float pointLights_data[7][4];
                    {
                        //pointLights[0].constant
                        pointLights_data[1][0] = 1.0f;
                        pointLights_data[1][1] = 0.0f;
                        pointLights_data[1][2] = 0.0f;
                        pointLights_data[1][3] = 0.0f;
                        
                        //pointLights[0].linear
                        pointLights_data[2][0] = 0.09f;
                        pointLights_data[2][1] = 0.0f;
                        pointLights_data[2][2] = 0.0f;
                        pointLights_data[2][3] = 0.0f;
                        
                        //pointLights[0].quadratic
                        pointLights_data[3][0] = 0.032f;
                        pointLights_data[3][1] = 0.0f;
                        pointLights_data[3][2] = 0.0f;
                        pointLights_data[3][3] = 0.0f;
                        
                        //pointLights[0].ambient
                        pointLights_data[4][0] = 0.05f;
                        pointLights_data[4][1] = 0.05f;
                        pointLights_data[4][2] = 0.05f;
                        pointLights_data[4][3] = 0.0f;
                        
                        //pointLights[0].diffuse
                        pointLights_data[5][0] = 0.8f;
                        pointLights_data[5][1] = 0.8f;
                        pointLights_data[5][2] = 0.8f;
                        pointLights_data[5][3] = 0.0f;
                        
                        //pointLights[0].specular
                        pointLights_data[6][0] = 1.0f;
                        pointLights_data[6][1] = 1.0f;
                        pointLights_data[6][2] = 1.0f;
                        pointLights_data[6][3] = 0.0f;
                        
                        for (int i = 0; i < 4; i++) {
                            //pointLights[0].position
                            pointLights_data[0][0] = pointLightPositions[i].x;
                            pointLights_data[0][1] = pointLightPositions[i].y;
                            pointLights_data[0][2] = pointLightPositions[i].z;
                            pointLights_data[0][3] = 0.0f;
                            
                            bgfx::setUniform(pointLights[i], &pointLights_data, 7);
                        }
                    }
                    
                    bx::Vec3 camera_front = cameraGetAt();
                    float spotLight_data[10][4];
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
                        
                        //spotLight.cutOff
                        spotLight_data[2][0] = glm::cos(glm::radians(12.5f));
                        spotLight_data[2][1] = 0.0f;
                        spotLight_data[2][2] = 0.0f;
                        spotLight_data[2][3] = 0.0f;
                        
                        //spotLight.outerCutOff
                        spotLight_data[3][0] = glm::cos(glm::radians(15.0f));
                        spotLight_data[3][1] = 0.0f;
                        spotLight_data[3][2] = 0.0f;
                        spotLight_data[3][3] = 0.0f;
                        
                        //spotLight.constant
                        spotLight_data[4][0] = 1.0f;
                        spotLight_data[4][1] = 0.0f;
                        spotLight_data[4][2] = 0.0f;
                        spotLight_data[4][3] = 0.0f;
                        
                        //spotLight.linear
                        spotLight_data[5][0] = 0.09f;
                        spotLight_data[5][1] = 0.0f;
                        spotLight_data[5][2] = 0.0f;
                        spotLight_data[5][3] = 0.0f;
                        
                        //spotLight.quadratic
                        spotLight_data[6][0] = 0.032f;
                        spotLight_data[6][1] = 0.0f;
                        spotLight_data[6][2] = 0.0f;
                        spotLight_data[6][3] = 0.0f;
                        
                        //spotLight.ambient
                        spotLight_data[7][0] = 0.0f;
                        spotLight_data[7][1] = 0.0f;
                        spotLight_data[7][2] = 0.0f;
                        spotLight_data[7][3] = 0.0f;
                        
                        //spotLight.diffuse
                        spotLight_data[8][0] = 1.0f;
                        spotLight_data[8][1] = 1.0f;
                        spotLight_data[8][2] = 1.0f;
                        spotLight_data[8][3] = 0.0f;
                        
                        //spotLight.specular
                        spotLight_data[9][0] = 1.0f;
                        spotLight_data[9][1] = 1.0f;
                        spotLight_data[9][2] = 1.0f;
                        spotLight_data[9][3] = 0.0f;
                        
                        bgfx::setUniform(spotLight, &spotLight_data, 10);
                    }
                }
                
                for(unsigned int i = 0; i < 10; i++)
                {
                    // Submit Triangle.
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, cubePositions[i]);
                    float angle = 20.0f * i;
                    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                    
                    // Set model matrix for rendering.
                    bgfx::setTransform(glm::value_ptr(model));
                    glm::mat4 u_normal_matrix = glm::transpose(glm::inverse(model));
                    bgfx::setUniform(normal_matrix, &u_normal_matrix);
                    
                    // Set vertex and index buffer.
                    bgfx::setVertexBuffer(0, m_vbh);
                    bgfx::setIndexBuffer(ibh);
                    
                    // Set render states.
                    bgfx::setState(state);
                    
                    // Submit primitive for rendering to view 0.
                    bgfx::submit(0, m_program_box);
                }
            }
            
            //Main Light
            {
                for (int i = 0; i < 4; i++) {
                    // Submit Triangle.
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, pointLightPositions[i]);
                    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
                    // Set model matrix for rendering.
                    bgfx::setTransform(glm::value_ptr(model));
                    
                    // Set vertex and index buffer.
                    bgfx::setVertexBuffer(0, m_vbh);
                    bgfx::setIndexBuffer(ibh);
                    
                    // Set render states.
                    bgfx::setState(state);
                    
                    // Submit primitive for rendering to view 0.
                    bgfx::submit(0, m_program_light);
                }
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
    bgfx::VertexBufferHandle m_vbh;
    bgfx::IndexBufferHandle m_ibh[BX_COUNTOF(s_ptState)];
    bgfx::ProgramHandle m_program_box;
    bgfx::ProgramHandle m_program_light;
    
    int64_t m_timeOffset;
    int32_t m_pt;
    
    bool m_r;
    bool m_g;
    bool m_b;
    bool m_a;
    
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
    
    // lighting
    bgfx::UniformHandle viewPos;
    bgfx::UniformHandle normal_matrix;
    
    bgfx::UniformHandle s_diffuse;
    bgfx::TextureHandle m_diffuse;
    bgfx::UniformHandle s_specular;
    bgfx::TextureHandle m_specular;
    bgfx::UniformHandle material;
    
    bgfx::UniformHandle dirLight;
    bgfx::UniformHandle pointLights[4];
    bgfx::UniformHandle spotLight;
};

} // namespace

ENTRY_IMPLEMENT_MAIN(
                     ExampleLight
                     , "hello-light"
                     , "Rendering simple static mesh."
                     , "https://bkaradzic.github.io/bgfx/examples.html#cubes"
                     );

