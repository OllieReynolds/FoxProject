#pragma once

#include "FoxBaseScene.hpp"

class FoxScene : public FoxBaseScene {
    unsigned int VAO, VBO;
    unsigned int shaderProgram;
    unsigned int foxTexUnit;
    int numVertices;

public:
    void init() override;
    void update(float deltaTime) override;
    void render() override;
};
