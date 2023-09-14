// FoxBaseScene.cpp

#include "FoxBaseScene.hpp"
#include <iostream>

namespace fox {
    namespace scenes {
        void FoxBaseScene::setGlobalRocketSync(fox::utils::FoxRocketSync& globalRocketSync) {
            this->globalRocketSync = &globalRocketSync;
        }

        void FoxBaseScene::reloadShadersIfModified() {
            for (auto& [name, shaderInfo] : shaders) {
                // Get the current last modification times
                auto currentVertModTime = std::filesystem::last_write_time(shaderInfo.vertPath);
                auto currentFragModTime = std::filesystem::last_write_time(shaderInfo.fragPath);

                // Check if either shader file has been modified
                if (currentVertModTime != shaderInfo.lastModTimeVert || currentFragModTime != shaderInfo.lastModTimeFrag) {
                    // Delete the old program and related resources
                    glDeleteProgram(shaderInfo.program);

                    // Recompile and relink the shader
                    try {
                        shaderInfo.program = fox::utils::compileShader(
                            shaderInfo.vertPath,
                            shaderInfo.fragPath
                        );
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Failed to reload shader: " << e.what() << std::endl;
                        continue;
                    }

                    // Update the last modification times
                    shaderInfo.lastModTimeVert = currentVertModTime;
                    shaderInfo.lastModTimeFrag = currentFragModTime;

                    std::cout << "Reloaded shader: " << name << std::endl;
                }
            }
        }

        void FoxBaseScene::addShader(const std::string& name, const std::string& vertPath, const std::string& fragPath) {
            // Check if a shader with the given name already exists
            if (shaders.find(name) != shaders.end()) {
                std::cerr << "Shader with name '" << name << "' already exists." << std::endl;
                return;
            }

            // Compile the shader program
            unsigned int program;
            try {
                program = fox::utils::compileShader(vertPath, fragPath);
            }
            catch (const std::exception& e) {
                std::cerr << "Failed to add shader: " << e.what() << std::endl;
                return;
            }

            // Initialize the ShaderInfo struct
            ShaderInfo shaderInfo;
            shaderInfo.program = program;
            shaderInfo.vertPath = vertPath;
            shaderInfo.fragPath = fragPath;
            shaderInfo.lastModTimeVert = std::filesystem::last_write_time(vertPath);
            shaderInfo.lastModTimeFrag = std::filesystem::last_write_time(fragPath);

            // Add the ShaderInfo struct to the shaders map
            shaders[name] = shaderInfo;

            std::cout << "Added shader: " << name << std::endl;
        }

        unsigned int FoxBaseScene::getShaderProgram(const std::string& name) {
            if (shaders.find(name) == shaders.end()) {
                std::cerr << "Shader with name '" << name << "' not found." << std::endl;
                return 0;
            }
            return shaders[name].program;
        }

        void FoxBaseScene::setReloadShadersFlag(bool flag) {
            reloadShadersFlag = flag;
        }

        void FoxBaseScene::checkForShaderReload() {
            if (reloadShadersFlag) {
                reloadShadersIfModified();
                reloadShadersFlag = false;
            }
        }

        void FoxBaseScene::update(float deltaTime, GLFWwindow* window) {
            checkForShaderReload();
        }
    }
}
