#include "../include/Application/Application.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <iostream>
#include <cmath>
#include <sstream>  
#include <filesystem>

#include "../include/Buffers/VertexBuffer.h"
#include "../include/Buffers/VertexArray.h"
#include "../include/Buffers/IndexBuffer.h"
#include "../include/Shaders/Shader.h"
#include "../include/Textures/Texture.h"

#include "../include/Scenes/Scene.h"
#include "../include/Scenes/UserScenes/SpecularMaps.h"
#include "../include/Scenes/BaseScenes/Grid.h"
#include "../include/Application/ApplicationSettings.h"
#include "../include/Scenes/BaseScenes/CartesianCS.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

Application::Application()
{
    // Initialize settings after Application is fully constructed
    m_settings = new ApplicationSettings(this);

    /////////////////////Initialize dependencies/////////////////////
        // Initialize GLFW
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW\n";
        exit(EXIT_FAILURE);
    }

    // Set GLFW window properties
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // MSAA
    glfwWindowHint(GLFW_SAMPLES, m_settings->getSamplesPerFragment());

    m_MainWindow = glfwCreateWindow(m_windowWidth, m_windowHeight, "Test Site", NULL, NULL);
    if (m_MainWindow == NULL)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(m_MainWindow);

    // Load OpenGL function pointers with glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        exit(EXIT_FAILURE);
    }
    /////////////////////////////////////////////////////////////////

       // Set the viewport
    glViewport(0, 0, m_windowWidth, m_windowHeight);
    glfwSetFramebufferSizeCallback(m_MainWindow, Application::framebuffer_size_callback);

    // User Standart Settings 
    glfwSwapInterval(m_settings->isVSyncEnabled());

    //ImGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_MainWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");
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

    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

    Scene::Scene* crntScene = nullptr;
    Scene::SceneMenu* SceneMenu = new Scene::SceneMenu(m_settings, crntScene);
    crntScene = SceneMenu;

    Scene::CartesianCS* cartesianCS = new Scene::CartesianCS(m_settings);
    Scene::Grid* grid = new Scene::Grid(m_settings);

    SceneMenu->RegisterTest<Scene::SpecularMaps>("Specular Maps Showcase");

    static int frameCount, FPS;
    static double previousTime;
    while (!glfwWindowShouldClose(m_MainWindow))
    {
        glfwPollEvents();

        processInput();

        glEnable(GL_DEPTH_TEST); glEnable(GL_MULTISAMPLE);
        glDepthFunc(GL_LEQUAL);

        // Clear Scene
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Poligon Mode
        if (m_settings->isPolygonModeFill()) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        //ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /////////////////////////////////////////////////////////

        ShowWorldSettings();

        cartesianCS->OnRender();
        grid->OnRender();

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

    // FOV slider
    ImGui::SliderFloat("FOV", m_Camera.getFOVByRefferance(), 50.0f, 90.0f);

    // MSAA combo box
    const std::vector<std::string> msaaOptions = { "OFF", "2x MSAA", "4x MSAA", "8x MSAA" };
    static int currentMSAAOption = 2; // Default to 4x MSAA
    if (ImGui::BeginCombo("MSAA(dont work)", msaaOptions[currentMSAAOption].c_str()))
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

                glfwWindowHint(GLFW_SAMPLES, m_settings->getSamplesPerFragment());
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