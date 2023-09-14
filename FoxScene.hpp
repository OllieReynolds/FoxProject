#pragma once

// Project Includes
#include "FoxBaseScene.hpp"
#include "FoxCamera.hpp"
#include "FoxRocketSync.hpp"

// Forward Declarations
struct GLFWwindow;

// Namespace Declaration
namespace fox {
    namespace scenes {

        // Using Declarations
        using namespace fox::camera;

        // Class Declaration
        class FoxScene : public FoxBaseScene {
        public:
            // Constructors
            FoxScene();

            // Public Member Functions (Overridden from FoxBaseScene)
            void init(GLFWwindow* window) override;
            void update(float deltaTime, GLFWwindow* window) override;
            void render() override;
            void destroy() override;

        private:
            // Private Member Variables
            unsigned int VAO, VBO;
            unsigned int foxTexUnit;
            int numVertices;
            Camera camera;

            // Sync
            fox::utils::FoxRocketSync sceneSpecificRocket;
        };

    }  // namespace scenes
}  // namespace fox
