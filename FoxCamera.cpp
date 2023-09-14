#include "FoxCamera.hpp"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <algorithm>

namespace fox {
    namespace camera {
        Camera::Camera(int projection_w, int projection_h)
            : isRightMouseButtonDown(false),
            cameraPos(glm::vec3(0.0f, 0.0f, 3.0f)),
            cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)),
            cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)),
            lastX(400),
            lastY(300),
            yaw(-90.0f),
            pitch(0.0f),
            firstMouse(true),
            projection{ glm::perspective(glm::radians(90.0f), (float)projection_w / (float)projection_h, 0.1f, 1000.0f) }
        { }

        void Camera::setRightMouseButtonDown(bool state) {
            isRightMouseButtonDown = state;
        }

        bool Camera::getRightMouseButtonDown() const {
            return isRightMouseButtonDown;
        }

        glm::mat4 Camera::getViewMatrix() const {
            return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        }

        glm::mat4 Camera::getProjectionMatrix() const {
            return projection;
        }

        void Camera::processKeyboard(GLFWwindow* window, float deltaTime) {
            float cameraSpeed = 40.5f * deltaTime;
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                cameraPos += cameraSpeed * cameraFront;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                cameraPos -= cameraSpeed * cameraFront;
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }

        glm::vec3 Camera::getPosition() const {
            return cameraPos;
        }

        void Camera::processMouseMovement(double xpos, double ypos) {
            if (firstMouse) {
                lastX = xpos;
                lastY = ypos;
                firstMouse = false;
            }

            float xoffset = xpos - lastX;
            float yoffset = lastY - ypos;
            lastX = xpos;
            lastY = ypos;

            float sensitivity = 0.1f;
            xoffset *= sensitivity;
            yoffset *= sensitivity;

            yaw += xoffset;
            pitch += yoffset;

            pitch = std::clamp(pitch, -89.0f, 89.0f);

            glm::vec3 front;
            front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            front.y = sin(glm::radians(pitch));
            front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            cameraFront = glm::normalize(front);
        }
    }
}