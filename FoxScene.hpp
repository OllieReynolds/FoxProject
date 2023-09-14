#pragma once

#include "FoxBaseScene.hpp"
#include "FoxCamera.hpp"
#include "FoxRocketSync.hpp"

namespace fox {
    namespace scenes {
        using namespace fox::camera;

        class FoxScene : public FoxBaseScene {
            unsigned int VAO, VBO;
            unsigned int foxTexUnit;
            int numVertices;
            Camera camera;

            fox::utils::FoxRocketSync sceneSpecificRocket;

        public:
            FoxScene();

            void init(GLFWwindow* window) override;
            void update(float deltaTime, GLFWwindow* window) override;
            void render() override;
            void destroy() override;
        };
    }
}
