#include "FoxScene.hpp"
#include "FoxUtils.hpp"
#include "FoxGlobals.hpp"

#include <stdexcept>

namespace fox {
    namespace scenes {
        FoxScene::FoxScene() : camera{Camera(SCR_WIDTH, SCR_HEIGHT)}, sceneSpecificRocket("FoxScene_tracks.rocket")
        {}

        void FoxScene::init(GLFWwindow* window) {
            glfwSetCursorPosCallback(window, fox::utils::mouse_callback);
            glfwSetMouseButtonCallback(window, fox::utils::mouse_button_callback);
            glfwSetWindowUserPointer(window, &camera);

            tinygltf::Model model;
            fox::utils::GLTFModelData modelData;

            if (!fox::utils::LoadGLTFModel("C:\\Users\\cubed\\FoxThing\\data\\models\\fox\\scene.gltf", model, modelData)) {
                throw std::runtime_error("Failed to load GLTF model");
            }

            shaderProgram = fox::utils::compileShader(
                "C:\\Users\\cubed\\FoxThing\\data\\shaders\\test.vert",
                "C:\\Users\\cubed\\FoxThing\\data\\shaders\\test.frag"
            );

            numVertices = modelData.posAccessor.count;

            std::vector<float> positionVec(modelData.positionBuffer, modelData.positionBuffer + numVertices * 3);
            std::vector<float> normalVec(modelData.normalBuffer, modelData.normalBuffer + numVertices * 3);
            std::vector<float> texCoordVec(modelData.texCoordBuffer, modelData.texCoordBuffer + numVertices * 2);
            std::vector<float> interleavedData = fox::utils::interleaveData(positionVec, normalVec, texCoordVec, 8);

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, interleavedData.size() * sizeof(float), &interleavedData[0], GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);

            foxTexUnit = fox::utils::loadTexture("C:\\Users\\cubed\\FoxThing\\data\\models\\fox\\textures\\fox_material_baseColor.png");
        }

        void FoxScene::update(float deltaTime, GLFWwindow* window) {
            sceneSpecificRocket.update(0.0);

            fox::utils::processInput(window);
            camera.processKeyboard(window, deltaTime);
        }

        void FoxScene::render() {
            glClearColor(0.7f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glUseProgram(shaderProgram);

            if (globalRocketSync) {
                float globalLightIntensity = globalRocketSync->getTrackValue("global.lightIntensity", 0.0);
            }

            float sceneSpecificParameter = sceneSpecificRocket.getTrackValue("scene.specificParameter", 0.0);

            glm::vec3 cameraPos = camera.getPosition();
            int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
            glUniform3f(lightPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);

            glm::mat4 view = camera.getViewMatrix();
            int viewLoc = glGetUniformLocation(shaderProgram, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

            int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));

            glm::mat4 model = glm::mat4(1.f);
            model = glm::scale(model, glm::vec3(0.1f));
            int modelLoc = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, foxTexUnit);
            glUniform1i(glGetUniformLocation(shaderProgram, "foxTex"), 0);

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, numVertices);
        }

        void FoxScene::destroy() {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteProgram(shaderProgram);
        }
    }
}
