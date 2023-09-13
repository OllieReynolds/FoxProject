#include "FoxUtils.hpp"
#include <gtc/matrix_transform.hpp>
#include <algorithm>

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include "stb_image.h"

#include "glad.h"

namespace fox {
    namespace utils {
        void GLAPIENTRY MessageCallback(GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar* message,
            const void* userParam) {
            fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
                (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
                type, severity, message);
        }

        void processInput(GLFWwindow* window) {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);
        }

        void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
            glViewport(0, 0, width, height);
        }

        void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
            Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
            if (!camera->getRightMouseButtonDown()) {
                return;
            }
            camera->processMouseMovement(xpos, ypos);
        }

        void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
            Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
            if (button == GLFW_MOUSE_BUTTON_RIGHT) {
                if (action == GLFW_PRESS) {
                    camera->setRightMouseButtonDown(true);

                    // Get the framebuffer size
                    int width, height;
                    glfwGetFramebufferSize(window, &width, &height);
                    double xCenter = width / 2.0;
                    double yCenter = height / 2.0;

                    // Set cursor position to the center
                    glfwSetCursorPos(window, xCenter, yCenter);
                    // Optionally lock the cursor
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

                }
                else if (action == GLFW_RELEASE) {
                    camera->setRightMouseButtonDown(false);
                    // Optionally unlock the cursor
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                }
            }
        }



        Camera::Camera()
            : isRightMouseButtonDown(false),
            cameraPos(glm::vec3(0.0f, 0.0f, 3.0f)),
            cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)),
            cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)),
            lastX(400),
            lastY(300),
            yaw(-90.0f),
            pitch(0.0f),
            firstMouse(true) {}

        void Camera::setRightMouseButtonDown(bool state) {
            isRightMouseButtonDown = state;
        }

        bool Camera::getRightMouseButtonDown() const {
            return isRightMouseButtonDown;
        }

        glm::mat4 Camera::getViewMatrix() const {
            return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        }

        void Camera::processKeyboard(GLFWwindow* window, float deltaTime) {
            float cameraSpeed = 20.5f * deltaTime;
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


        std::string readFile(const std::string& filePath) {
            std::ifstream shaderFile(filePath);
            std::stringstream shaderStream;
            shaderStream << shaderFile.rdbuf();
            shaderFile.close();
            return shaderStream.str();
        }

        unsigned int loadTexture(char const* path) {
            unsigned int textureID;
            glGenTextures(1, &textureID);

            int width, height, nrComponents;
            unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
            if (data)
            {
                GLenum format;
                if (nrComponents == 1)
                    format = GL_RED;
                else if (nrComponents == 3)
                    format = GL_RGB;
                else if (nrComponents == 4)
                    format = GL_RGBA;

                glBindTexture(GL_TEXTURE_2D, textureID);
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                stbi_image_free(data);
            }
            else
            {
                std::cout << "Texture failed to load at path: " << path << std::endl;
                stbi_image_free(data);
            }

            return textureID;
        }

        bool LoadGLTFModel(const std::string& filename, tinygltf::Model& model, GLTFModelData& modelData) {
            tinygltf::TinyGLTF loader;
            std::string err, warn;

            bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, filename);

            if (!warn.empty()) std::cout << "Warn: " << warn << std::endl;
            if (!err.empty()) std::cout << "Err: " << err << std::endl;
            if (!ret) return false;

            if (model.meshes.empty()) {
                std::cout << "No meshes found in model." << std::endl;
                return false;
            }

            tinygltf::Mesh& mesh = model.meshes[0];
            if (mesh.primitives.empty()) {
                std::cout << "No primitives found in mesh." << std::endl;
                return false;
            }

            tinygltf::Primitive& primitive = mesh.primitives[0];

            if (primitive.attributes.find("POSITION") == primitive.attributes.end() ||
                primitive.attributes.find("NORMAL") == primitive.attributes.end()) {
                std::cout << "POSITION or NORMAL attributes not found in primitive." << std::endl;
                return false;
            }

            int posAccessorIdx = primitive.attributes.at("POSITION");
            tinygltf::Accessor& posAccessor = model.accessors[posAccessorIdx];
            tinygltf::BufferView& posView = model.bufferViews[posAccessor.bufferView];
            modelData.positionBuffer = reinterpret_cast<float*>(&(model.buffers[posView.buffer].data[posView.byteOffset]));

            int normalAccessorIdx = primitive.attributes.at("NORMAL");
            tinygltf::Accessor& normalAccessor = model.accessors[normalAccessorIdx];
            tinygltf::BufferView& normalView = model.bufferViews[normalAccessor.bufferView];
            modelData.normalBuffer = reinterpret_cast<float*>(&(model.buffers[normalView.buffer].data[normalView.byteOffset]));

            // Check for texture coordinates
            if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end()) {
                int texCoordAccessorIdx = primitive.attributes.at("TEXCOORD_0");
                tinygltf::Accessor& texCoordAccessor = model.accessors[texCoordAccessorIdx];
                tinygltf::BufferView& texCoordView = model.bufferViews[texCoordAccessor.bufferView];
                modelData.texCoordBuffer = reinterpret_cast<float*>(&(model.buffers[texCoordView.buffer].data[texCoordView.byteOffset]));
            }

            modelData.posAccessor = posAccessor;

            // Load animations
            for (const auto& anim : model.animations) {
                Animation animation;
                animation.name = anim.name;

                // Loop through all channels in the animation
                for (const auto& channel : anim.channels) {
                    AnimationChannel animChannel;

                    // Node that this channel targets
                    animChannel.nodeIndex = channel.target_node;

                    // Sampler for this channel
                    const auto& sampler = anim.samplers[channel.sampler];

                    // Load keyframe data
                    const auto& inputAccessor = model.accessors[sampler.input];
                    const auto& outputAccessor = model.accessors[sampler.output];

                    const auto& inputView = model.bufferViews[inputAccessor.bufferView];
                    const auto& outputView = model.bufferViews[outputAccessor.bufferView];

                    const float* keyframeTimestamps = reinterpret_cast<const float*>(&(model.buffers[inputView.buffer].data[inputView.byteOffset]));
                    const float* keyframeData = reinterpret_cast<const float*>(&(model.buffers[outputView.buffer].data[outputView.byteOffset]));

                    for (size_t i = 0; i < inputAccessor.count; ++i) {
                        AnimationKeyframe keyframe;

                        // Timestamp for this keyframe
                        keyframe.timestamp = keyframeTimestamps[i];

                        // Depending on the target path (translation/rotation/scale), the number of components will differ
                        if (channel.target_path == "translation") {
                            keyframe.position = glm::vec3(keyframeData[i * 3], keyframeData[i * 3 + 1], keyframeData[i * 3 + 2]);
                        }
                        else if (channel.target_path == "rotation") {
                            keyframe.rotation = glm::quat(keyframeData[i * 4], keyframeData[i * 4 + 1], keyframeData[i * 4 + 2], keyframeData[i * 4 + 3]);
                        }
                        else if (channel.target_path == "scale") {
                            keyframe.scale = glm::vec3(keyframeData[i * 3], keyframeData[i * 3 + 1], keyframeData[i * 3 + 2]);
                        }

                        animChannel.keyframes.push_back(keyframe);
                    }

                    animation.channels.push_back(animChannel);
                }

                // TODO: Compute the duration of the animation based on the keyframe timestamps

                modelData.animations.push_back(animation);
            }


            return true;
        }

        glm::mat4 interpolateAnimation(const Animation& animation, float currentTime) {
            glm::mat4 nodeTransform = glm::mat4(1.0f);

            if (!animation.channels.empty()) {
                const auto& channel = animation.channels[0];

                AnimationKeyframe prevKeyframe;
                AnimationKeyframe nextKeyframe;
                float factor = 0.0f;

                for (size_t i = 0; i < channel.keyframes.size(); ++i) {
                    if (channel.keyframes[i].timestamp > currentTime) {
                        nextKeyframe = channel.keyframes[i];
                        if (i > 0) {
                            prevKeyframe = channel.keyframes[i - 1];
                        }
                        factor = (currentTime - prevKeyframe.timestamp) / (nextKeyframe.timestamp - prevKeyframe.timestamp);
                        break;
                    }
                }

                // Interpolate position, rotation, and scale
                glm::vec3 interpolatedPosition = glm::mix(prevKeyframe.position, nextKeyframe.position, factor);
                glm::quat interpolatedRotation = glm::slerp(prevKeyframe.rotation, nextKeyframe.rotation, factor);
                glm::vec3 interpolatedScale = glm::mix(prevKeyframe.scale, nextKeyframe.scale, factor);

                // Create transformation matrices
                glm::mat4 translation = glm::translate(glm::mat4(1.0f), interpolatedPosition);
                glm::mat4 rotation = glm::mat4_cast(interpolatedRotation);
                glm::mat4 scale = glm::scale(glm::mat4(1.0f), interpolatedScale);

                nodeTransform = translation * rotation * scale;
            }

            return nodeTransform;
        }

        unsigned int compileShader(const std::string& vertexFilePath, const std::string& fragmentFilePath) {
            std::string vertexShaderCode = readFile(vertexFilePath);
            std::string fragmentShaderCode = readFile(fragmentFilePath);
            const char* vertexShaderSource = vertexShaderCode.c_str();
            const char* fragmentShaderSource = fragmentShaderCode.c_str();

            unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
            glCompileShader(vertexShader);
            int success;
            char infoLog[512];
            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            }

            unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
            glCompileShader(fragmentShader);
            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
            }

            unsigned int shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);
            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            }

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            return shaderProgram;
        }

        std::vector<float> interleaveData(const std::vector<float>& positions, const std::vector<float>& normals, const std::vector<float>& texCoords, int vertexSize) {
            int numVertices = positions.size() / 3;
            std::vector<float> interleavedData(numVertices * vertexSize);

            for (int i = 0; i < numVertices; ++i) {
                // Insert position
                interleavedData[i * vertexSize] = positions[i * 3];
                interleavedData[i * vertexSize + 1] = positions[i * 3 + 1];
                interleavedData[i * vertexSize + 2] = positions[i * 3 + 2];

                // Insert normal
                interleavedData[i * vertexSize + 3] = normals[i * 3];
                interleavedData[i * vertexSize + 4] = normals[i * 3 + 1];
                interleavedData[i * vertexSize + 5] = normals[i * 3 + 2];

                // Insert texture coordinates
                interleavedData[i * vertexSize + 6] = texCoords[i * 2];  // x-coordinate
                interleavedData[i * vertexSize + 7] = texCoords[i * 2 + 1];  // y-coordinate
            }

            return interleavedData;
        }

        void CheckGLError() {
            GLenum err;
            while ((err = glGetError()) != GL_NO_ERROR) {
                std::cerr << "OpenGL error: " << err << std::endl;
            }
        }
    }
}