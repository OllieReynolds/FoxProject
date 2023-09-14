#pragma once

// Define Flags
#define GLFW_INCLUDE_NONE

// Standard Library Includes
#include <string>
#include <vector>

// External Libraries
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/quaternion.hpp>
#include "tiny_gltf.h"
#include "glad.h"

// Forward Declarations
namespace fox {
    namespace scenes {
        class FoxBaseScene;
    }
}

// Namespace Declaration
namespace fox {
    namespace utils {

        // Callback Functions
        void mouse_callback(GLFWwindow* window, double xpos, double ypos);
        void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        void framebuffer_size_callback(GLFWwindow* window, int width, int height);

        // Input Processing
        void processInput(GLFWwindow* window, fox::scenes::FoxBaseScene& scene);

        // Debugging
        void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

        // File I/O
        std::string readFile(const std::string& filePath);

        // Structs
        struct GLTFNode {
            glm::mat4 transform;
            std::vector<int> meshIndices;
            std::vector<glm::vec3> translationKeys;
            std::vector<glm::quat> rotationKeys;
            std::vector<glm::vec3> scaleKeys;
            std::vector<float> keyTimes;
        };

        struct GLTFModelData {
            float* positionBuffer;
            float* normalBuffer;
            float* texCoordBuffer;
            tinygltf::Accessor posAccessor;
        };

        // Utility Functions
        unsigned int loadTexture(char const* path);
        bool LoadGLTFModel(const std::string& filename, tinygltf::Model& model, GLTFModelData& modelData);
        unsigned int compileShader(const std::string& vertexFilePath, const std::string& fragmentFilePath);
        std::vector<float> interleaveData(const std::vector<float>& positions, const std::vector<float>& normals, const std::vector<float>& texCoords, int vertexSize);

        // Error Checking
        void CheckGLError();

    }  // namespace utils
}  // namespace fox
