#pragma once

class FoxBaseScene {
public:
    virtual void init() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
};