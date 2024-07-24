#include "Scenes/Scene.h"
#include "Debugging/Debugging.h"

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

    void Scene::UpdateBasicSettings(Shader shader)
    {
        shader.Bind();

        model = glm::mat4(1.0f);
        view = m_ApplicationSettings->application->getCamera().GetViewMatrix();
        projection = m_ApplicationSettings->application->GetProjectionMatrix();

        shader.setMat4("u_ModelMatrix", model);
        shader.setMat4("u_ViewMatrix", view);
        shader.setMat4("u_Projection", projection);

        viewPos = m_ApplicationSettings->application->getCamera().GetPosition();
        shader.setVec3("u_ViewPos", viewPos);

        // Directional light
        shader.setBool("u_DirectionLightEnabled", isDirectionLightEnabled);
        if (isDirectionLightEnabled) directionLight.SetShaderUniforms(shader);

        // Point lights
        for (int i = 0; i < pointLights.size(); i++) { pointLights[i].SetShaderUniforms(shader, i); }

        // Spot Lights
        for (int i = 0; i < spotLights.size(); i++) { spotLights[i].SetShaderUniforms(shader, i); }
        //

        shader.setInt("u_NumPointLights", pointLights.size());
        shader.setInt("u_NumSpotLights", spotLights.size());
    }

    void Scene::RenderBasicElements(Shader shader)
    {
        shader.Bind();

        for (size_t i = 0; i < pointLights.size(); i++)
            GL_CHECK(pointLights[i].Render(view, projection, viewPos));

        for (size_t i = 0; i < spotLights.size(); i++)
            GL_CHECK(spotLights[i].Render(view, projection));

        shader.Unbind();
    }
}