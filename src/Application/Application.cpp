#include "../include/Application/Application.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <cmath>
#include <filesystem>
#include <iostream>
#include <sstream>  

#include "Buffers/IndexBuffer.h"
#include "Buffers/VertexArray.h"
#include "Buffers/VertexBuffer.h"
#include "Shaders/Shader.h"
#include "Textures/Texture.h"
#include "Application/ApplicationSettings.h"
#include "Scenes/BaseScenes/CartesianCS.h"
#include "Scenes/Scene.h"
#include "Scenes/UserScenes/BackpackModel.h"
#include "Scenes/UserScenes/SpecularMaps.h"
#include "Buffers/FrameBuffer.h"
#include "Debugging/Logger.h"


void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
static void error_callback(int error, const char* description)
{
    std::cerr << FATAL_ERROR("ERROR::GLFW_error_callback::" + std::to_string(error) + ": " + description);
}

Application::Application()
{
    Logger::Initialize();

    // Initialize settings after Application is fully constructed
    m_settings = new ApplicationSettings(this);

    // Initialize GLFW
    if (!glfwInit()) { std::cerr << FATAL_ERROR("Failed to initialize GLFW"); exit(EXIT_FAILURE); }\

    // Set the error callback function
    glfwSetErrorCallback(error_callback);

    // Set GLFW window properties
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 32);

    m_MainWindow = glfwCreateWindow(m_windowWidth, m_windowHeight, "3D", NULL, NULL);
    if (m_MainWindow == NULL)
    {
        std::cerr << FATAL_ERROR("Failed to create GLFW window");
        glfwTerminate(); exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(m_MainWindow);

    // Load OpenGL function pointers with glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << FATAL_ERROR("Failed to initialize GLAD");
        exit(EXIT_FAILURE);
    }

    // Set the viewport
    glViewport(0, 0, m_windowWidth, m_windowHeight);
    glfwSetFramebufferSizeCallback(m_MainWindow, Application::framebuffer_size_callback);

    // User Standart Settings 
    glfwSwapInterval(m_settings->isVSyncEnabled());

    //ImGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_MainWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    // Show Device Settings
    Logger::WriteLog("Device Settings:");
    Logger::WriteLog("\tVendor graphic card: " + std::string(reinterpret_cast<const char*>(glGetString(GL_VENDOR))));
    Logger::WriteLog("\tRenderer: "            + std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER))));
    Logger::WriteLog("\tVersion GL: "          + std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))));
    Logger::WriteLog("\tVersion GLSL: "        + std::string(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION))) + "\n");

    Logger::WriteLog("GLFW/GLAD Initialized successfully");
}

Application::~Application()
{
    delete m_settings;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_MainWindow);
    glfwTerminate();
}

void Application::Run()
{
    int width = m_settings->application->getWindowWidth();
    int height = m_settings->application->getWindowHeight();

    // Frame Buffer Set-up
        FrameBuffer FBO = FrameBuffer(width, height, m_settings->getSamplesPerFragment());
        FrameBuffer postProcessingFBO = FrameBuffer(width, height);
        Shader fbShader("fbShader", "./resources/shaders/fbVert.glsl", "./resources/shaders/fbFrag.glsl");

        VertexArray screenVAO; VertexBuffer screenVBO = VertexBuffer(FBO.quadVertices, sizeof(FBO.quadVertices));
        screenVAO.Bind(); screenVBO.Bind();
        screenVAO.LinkAttrib(screenVBO, 0, 2, GL_FLOAT, 4 * sizeof(float), (void*)0);
        screenVAO.LinkAttrib(screenVBO, 1, 2, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        screenVAO.Unbind(); screenVBO.Unbind();

        fbShader.Bind();
        glUniform1i(glGetUniformLocation(fbShader.m_RendererID, "screenTexture"), 0);
        fbShader.Unbind();

        glCheckError();
    //

    Scene::Scene* crntScene = nullptr;
    Scene::SceneMenu* SceneMenu = new Scene::SceneMenu(m_settings, crntScene);
    crntScene = SceneMenu;

    Scene::CartesianCS* cartesianCS = new Scene::CartesianCS(m_settings);

    SceneMenu->RegisterTest<Scene::SpecularMaps>("Specular Maps Showcase");
    SceneMenu->RegisterTest<Scene::BackpackModel>("Model Showcase");

    // OpenGL settings
    glDepthFunc(GL_LEQUAL); // DEPTH

    glEnable(GL_MULTISAMPLE); // MASS

    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // BLENDING

    static int frameCount = 0, FPS = 0;
    static double previousTime = 0.0;
    while (!glfwWindowShouldClose(m_MainWindow))
    {
        glCheckError();

        if (m_settings->isMSAAChanged())
        {
            FBO.Reset(width, height, m_settings->getSamplesPerFragment());
            postProcessingFBO.Reset(width, height);
            m_settings->setMSAAChanged(false);
        }

        glCheckError();

        glfwPollEvents();

        processInput();

        FBO.Bind(); glCheckError();

        Renderer::ClearFrame(glm::vec3(0.3, 0.3, 0.3));

        glEnable(GL_DEPTH_TEST);

        glCheckError();

        // ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Create a dock space
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
        }

        /////////////////////////////////////////////////////////

        // Poligon Mode
        if (m_settings->isPolygonModeFill()) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        ShowWorldSettings();

        glCheckError();

        ShowSystemSettings();

        cartesianCS->OnRender();

        glCheckError();
        if (crntScene)
        {
            crntScene->OnUpdate(0.0f);
            crntScene->OnRender();

            ImGui::Begin("Menu");
            if (crntScene != SceneMenu && ImGui::Button("Back"))
            {
                delete crntScene;
                crntScene = SceneMenu;
            }
            crntScene->OnImGuirender();
            ImGui::End();
        }

        /////////////////////////////////////////////////////////

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glCheckError();

        glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO.m_RendererID); 
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postProcessingFBO.m_RendererID);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        glCheckError();

        FBO.Unbind();  glCheckError();

        fbShader.Bind();
        screenVAO.Bind();
        glDisable(GL_DEPTH_TEST);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, postProcessingFBO.framebufferTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        fbShader.Unbind();
        screenVAO.Unbind();

        glCheckError();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_MainWindow);

        frameCount++;
        double currentTime = glfwGetTime();
        double elapsedTime = currentTime - previousTime;
        if (elapsedTime >= 1.0)
        {
            FPS = (int)(frameCount / elapsedTime);
            previousTime = currentTime;
            frameCount = 0;

            std::stringstream ss; ss << "3D | FPS: " << FPS;
            glfwSetWindowTitle(m_MainWindow, ss.str().c_str());
        }
    }

    delete cartesianCS;
}

void Application::ShowWorldSettings()
{
    ImGui::Begin("Worlds Settings");

    if (ImGui::CollapsingHeader("Camera Settings"))
    {
        // FOV slider
        ImGui::SliderInt("FOV", m_Camera.getFOVByRefferance(), 45.0f, 90.0f);

        // Camera mod (orthogonal/perspective)
        ImGui::Checkbox("Ortogonal view (TEST OPTION)", &m_Camera.isOrtogonal);
    }

    ImGui::End();
}

void Application::ShowSystemSettings()
{
    ImGui::Begin("System Settings");

    // MSAA combo box
    const std::vector<std::string> msaaOptions = { "OFF", "2x MSAA", "4x MSAA", "8x MSAA" };
    static int currentMSAAOption = 0;
    int samples = m_settings->getSamplesPerFragment();
    if (samples == 2) currentMSAAOption = 1;
    else if (samples == 4) currentMSAAOption = 2;
    else if (samples == 8) currentMSAAOption = 3;
    if (ImGui::BeginCombo("MSAA", msaaOptions[currentMSAAOption].c_str()))
    {
        for (int n = 0; n < msaaOptions.size(); n++)
        {
            bool isSelected = (currentMSAAOption == n);
            if (ImGui::Selectable(msaaOptions[n].c_str(), isSelected))
            {
                currentMSAAOption = n;
                switch (currentMSAAOption)
                {
                case 0:
                    m_settings->setSamplesPerFragment(0);
                    break;
                case 1:
                    m_settings->setSamplesPerFragment(2);
                    break;
                case 2:
                    m_settings->setSamplesPerFragment(4);
                    break;
                case 3:
                    m_settings->setSamplesPerFragment(8);
                    break;
                }
                m_settings->setMSAAChanged(true);
            }
            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    ImGui::End();
}

void Application::processInput()
{
    if (glfwGetKey(m_MainWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_MainWindow, true);

    if (glfwGetKey(m_MainWindow, GLFW_KEY_V) == GLFW_PRESS)
        m_settings->setPolygonModeFill(false);
    if (glfwGetKey(m_MainWindow, GLFW_KEY_V) == GLFW_RELEASE)
        m_settings->setPolygonModeFill(true);

    m_Camera.Inputs(m_MainWindow);
}