#include "Application.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <cmath>
#include <sstream>  

#include "../Buffers/VertexBuffer.h"
#include "../Buffers/VertexArray.h"
#include "../Buffers/IndexBuffer.h"
#include "../Shaders/Shader.h"
#include "../Textures/Texture.h"

#include "../Scenes/Scene.h"
#include "../Scenes/UserScenes/SpecularMaps.h"
#include "../Scenes/BaseScenes/Grid.h"
#include "ApplicationSettings.h"
#include "../Scenes/BaseScenes/CartesianCS.h"

void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

Application::Application()
{
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

    // Initialize settings after Application is fully constructed
    m_settings = new ApplicationSettings(this);

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

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (m_settings->isPolygonModeFill()) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        //ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        /////////////////////////////////////////////////////////

        //ImGui
        ImGui::Begin("Worlds Settings");
        ImGui::SliderFloat("FOV", m_Camera.getFOVByRefferance(), 50.0f, 90.0f);
        ImGui::End();

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