#pragma once

#define GLFW_INCLUDE_NONE

#include <string>
#include "tiny_gltf.h"

#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/quaternion.hpp>

#include "glad.h"

namespace fox_utils {
    class Camera {
    public:
        Camera();

        glm::mat4 getViewMatrix() const;
        glm::vec3 getPosition() const;
        void processKeyboard(GLFWwindow* window, float deltaTime);
        void processMouseMovement(double xpos, double ypos);

        void setRightMouseButtonDown(bool state);
        bool getRightMouseButtonDown() const;

    private:
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

    struct AnimationKeyframe {
        float timestamp;
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;

        AnimationKeyframe() : timestamp(0.0f), position(0.0f), rotation(1.0f, 0.0f, 0.0f, 0.0f), scale(1.0f) {}
    };

    struct AnimationChannel {
        int nodeIndex;
        std::vector<AnimationKeyframe> keyframes;
    };

    struct Animation {
        std::string name;
        float duration;
        std::vector<AnimationChannel> channels;
    };

    glm::mat4 interpolateAnimation(const Animation& animation, float currentTime);

    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    void processInput(GLFWwindow* window);

    void GLAPIENTRY MessageCallback(GLenum source, GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam);

    std::string readFile(const std::string& filePath);

    struct GLTFModelData {
        float* positionBuffer;
        float* normalBuffer;
        float* texCoordBuffer;
        tinygltf::Accessor posAccessor;
        std::vector<Animation> animations;
    };

    unsigned int loadTexture(char const* path);
    bool LoadGLTFModel(const std::string& filename, tinygltf::Model& model, GLTFModelData& modelData);

    unsigned int compileShader(const std::string& vertexFilePath, const std::string& fragmentFilePath);

    std::vector<float> interleaveData(const std::vector<float>& positions, const std::vector<float>& normals, const std::vector<float>& texCoords, int vertexSize);

    void CheckGLError();
}
