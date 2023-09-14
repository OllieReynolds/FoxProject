#include "FoxScene.hpp"
#include "FoxUtils.hpp"
#include "FoxGlobals.hpp"
#include "FoxRocketTrackNames.hpp"
#include "FoxConfigurationManager.hpp"

#include <stdexcept>

namespace fox {
    namespace scenes {
        FoxScene::FoxScene() : foxTexUnit{ 0 }, camera { Camera(SCR_WIDTH, SCR_HEIGHT) }, sceneSpecificRocket(fox::utils::RocketTracks::SCENE_SPECIFIC_PARAMETER.c_str())
        {}

        void FoxScene::init(GLFWwindow* window) {
            glfwSetCursorPosCallback(window, fox::utils::mouse_callback);
            glfwSetMouseButtonCallback(window, fox::utils::mouse_button_callback);
            glfwSetWindowUserPointer(window, &camera);

            mesh = std::make_unique<fox::utils::FoxMesh>(fox::utils::getConfig("modelPath"));

            addShader("main", fox::utils::getConfig("vertShaderPath"), fox::utils::getConfig("fragShaderPath"), fox::utils::getConfig("geomShaderPath"));

            foxTexUnit = fox::utils::loadTexture(fox::utils::getConfig("texturePath").c_str());
        }

        void FoxScene::update(float deltaTime, GLFWwindow* window) {
            fox::utils::processInput(window, *this);

            sceneSpecificRocket.update(0.0);

            camera.processKeyboard(window, deltaTime);

            reloadShadersIfModified();
        }

        void FoxScene::render() {
            glClearColor(0.7f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            unsigned int shaderProgram = getShaderProgram("main");
            if (shaderProgram == 0) {
                return;
            }

            glUseProgram(shaderProgram);

            if (globalRocketSync) {
                float globalLightIntensity = globalRocketSync->getTrackValue(fox::utils::RocketTracks::GLOBAL_LIGHT_INTENSITY.c_str(), 0.0);
            }

            float sceneSpecificParameter = sceneSpecificRocket.getTrackValue(fox::utils::RocketTracks::SCENE_SPECIFIC_PARAMETER.c_str(), 0.0);

            glm::vec3 cameraPos = camera.getPosition();
            int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
            glUniform3f(lightPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);

            glm::mat4 view = camera.getViewMatrix();
            int viewLoc = glGetUniformLocation(shaderProgram, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

            int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));

            glm::mat4 model = glm::mat4(1.f);
            model = glm::scale(model, glm::vec3(1.f));
            int modelLoc = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, foxTexUnit);
            glUniform1i(glGetUniformLocation(shaderProgram, "foxTex"), 0);

            mesh->draw();
        }

        void FoxScene::destroy() {
            mesh->destroy();

            for (const auto& [name, shaderInfo] : shaders) {
                glDeleteProgram(shaderInfo.program);
            }

            shaders.clear();
        }
    }
}
