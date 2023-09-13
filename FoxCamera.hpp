#pragma once

#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/quaternion.hpp>

namespace fox {
    namespace camera {
        class Camera {
        public:
            Camera(int projection_w, int projection_h);

            glm::mat4 getViewMatrix() const;
            glm::vec3 getPosition() const;
            glm::mat4 getProjectionMatrix() const;
            void processKeyboard(GLFWwindow* window, float deltaTime);
            void processMouseMovement(double xpos, double ypos);

            void setRightMouseButtonDown(bool state);
            bool getRightMouseButtonDown() const;

        private:
            glm::mat4 projection;

            glm::vec3 cameraPos;
            glm::vec3 cameraFront;
            glm::vec3 cameraUp;

            float lastX;
            float lastY;
            float yaw;
            float pitch;

            bool isRightMouseButtonDown;
            bool firstMouse;
        };
    }
}