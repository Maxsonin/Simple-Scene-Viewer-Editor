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
    protected:
        ApplicationSettings* m_ApplicationSettings;

        // Light Settings
        std::vector<PointLight> pointLights;
        std::vector<SpotLight> spotLights;
        DirectionLight directionLight;
        bool isDirectionLightEnabled = false;
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
