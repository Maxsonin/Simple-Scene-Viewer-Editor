#pragma once

#include <imgui.h>
#include <vector>
#include <string>
#include <functional>

#include "Shaders/Shader.h"
#include "Buffers/VertexArray.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/IndexBuffer.h"
#include "Textures/Texture.h"
#include "Application/Application.h"
#include "Application/ApplicationSettings.h"
#include "Lightning/PointLight.h"
#include "Lightning/DirectionLight.h"
#include "Lightning/SpotLight.h"

namespace Scene
{
    class Scene
    {
    public:
        Scene(ApplicationSettings* applicationSettings)
            : m_ApplicationSettings(applicationSettings)
        { }
        virtual ~Scene() {};

        virtual void OnUpdate(float deltaTime) {}
        virtual void OnRender() {}
        virtual void OnImGuirender() {}

        void UpdateBasicSettings(Shader shader);
        void RenderBasicElements(Shader shader);
    protected:
        ApplicationSettings* m_ApplicationSettings;

        // Light Settings
        std::vector<PointLight> pointLights;
        std::vector<SpotLight>  spotLights;
        DirectionLight          directionLight;
        bool isDirectionLightEnabled = false;

        // MVP
        glm::mat4 model      = glm::mat4(1.0f);
        glm::mat4 view       = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        glm::vec3 viewPos    = glm::vec3(1.0f);
    };

    class SceneMenu : public Scene
    {
    public:
        SceneMenu(ApplicationSettings* appSet, Scene*& currentScene);

        void OnImGuirender() override;

        template<typename T>
        void RegisterTest(const std::string& name)
        {
            m_Scenes.push_back(std::make_pair(name, [this]() { return new T(this->m_ApplicationSettings); }));
        }

    private:
        Scene*& m_CurrentScene;
        std::vector<std::pair<std::string, std::function<Scene* ()>>> m_Scenes;
    };
}
