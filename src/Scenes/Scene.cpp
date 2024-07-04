#include "Scene.h"

namespace Scene
{
    SceneMenu::SceneMenu(ApplicationSettings* appSet, Scene*& currentScene)
        : Scene(appSet), m_CurrentScene(currentScene)
    {  }

    void SceneMenu::OnImGuirender()
    {
        for (auto& scene : m_Scenes)
        {
            if (ImGui::Button(scene.first.c_str()))
            {
                m_CurrentScene = scene.second();
            }
        }
    }
}