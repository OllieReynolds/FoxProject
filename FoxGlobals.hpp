#pragma once

#include "utils.hpp"

namespace {
    fox::utils::Camera camera;

    constexpr unsigned int SCR_WIDTH = 800;
    constexpr unsigned int SCR_HEIGHT = 600;

#ifdef _DEBUG
    auto fge = []() { fox_utils::CheckGLError(); };
#else
    auto fge = []() { /* Do nothing in release mode */ };
#endif
}