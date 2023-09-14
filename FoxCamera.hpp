#pragma once

// Standard Library Includes
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/quaternion.hpp>

// External Library Includes
#include <glfw3.h>

// Namespace Declaration
namespace fox {
    namespace camera {

        // Class Declaration
        class Camera {
        public:
            // Constructors
            Camera(int projection_w, int projection_h);

            // Public Member Functions
            glm::mat4 getViewMatrix() const;
            glm::vec3 getPosition() const;
            glm::mat4 getProjectionMatrix() const;
            void processKeyboard(GLFWwindow* window, float deltaTime);
            void processMouseMovement(double xpos, double ypos);
            void setRightMouseButtonDown(bool state);
            bool getRightMouseButtonDown() const;

        private:
            // Private Member Variables
            glm::mat4 projection;
            glm::vec3 cameraPos;
            glm::vec3 cameraFront;
            glm::vec3 cameraUp;

            // Camera State Variables
            float lastX;
            float lastY;
            float yaw;
            float pitch;

            // Flags
            bool isRightMouseButtonDown;
            bool firstMouse;
        };

    } // namespace camera
} // namespace fox
