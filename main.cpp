#define GLFW_INCLUDE_NONE

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#define _CRT_SECURE_NO_WARNINGS

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "glad.h"
#include <glfw3.h>
#include <iostream>

#include "utils.hpp"
#include "globals.hpp"

int main() {
    tinygltf::Model model;
    fox_utils::GLTFModelData modelData;

    if (!fox_utils::LoadGLTFModel("C:\\Users\\cubed\\FoxThing\\data\\models\\fox\\scene.gltf", model, modelData)) {
        return -1;
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, fox_utils::framebuffer_size_callback);
    glfwSetCursorPosCallback(window, fox_utils::mouse_callback);
    glfwSetMouseButtonCallback(window, fox_utils::mouse_button_callback);
    glfwSetWindowUserPointer(window, &camera);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(fox_utils::MessageCallback, 0);

    glEnable(GL_DEPTH_TEST);

    unsigned int shaderProgram = fox_utils::compileShader(
        "C:\\Users\\cubed\\FoxThing\\data\\shaders\\test.vert",
        "C:\\Users\\cubed\\FoxThing\\data\\shaders\\test.frag"
    );

    int numVertices = modelData.posAccessor.count;

    std::vector<float> positionVec(modelData.positionBuffer, modelData.positionBuffer + numVertices * 3);
    std::vector<float> normalVec(modelData.normalBuffer, modelData.normalBuffer + numVertices * 3);
    std::vector<float> texCoordVec(modelData.texCoordBuffer, modelData.texCoordBuffer + numVertices * 2);
    std::vector<float> interleavedData = fox_utils::interleaveData(positionVec, normalVec, texCoordVec, 8);

    unsigned int VBO, VAO;
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

    unsigned int foxTexUnit = fox_utils::loadTexture("C:\\Users\\cubed\\FoxThing\\data\\models\\fox\\textures\\fox_material_baseColor.png");

    glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    float lastFrame = 0.0f;
    float deltaTime = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        fox_utils::processInput(window);
        camera.processKeyboard(window, deltaTime);

        glClearColor(0.7f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::vec3 cameraPos = camera.getPosition();
        int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
        glUniform3f(lightPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);

        glm::mat4 view = camera.getViewMatrix();
        int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model = glm::mat4(1.f);
        model = glm::scale(model, glm::vec3(0.1f));
        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, foxTexUnit);
        glUniform1i(glGetUniformLocation(shaderProgram, "foxTex"), 0);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, numVertices);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

