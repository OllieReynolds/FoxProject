#pragma once

// Project Includes
#include "FoxUtils.hpp"

// Anonymous Namespace for Internal Constants and Utilities
namespace {

    // Screen Dimensions
    constexpr unsigned int SCR_WIDTH = 800;
    constexpr unsigned int SCR_HEIGHT = 600;

    // Error Checking Based on Build Mode
#ifdef _DEBUG
    auto fge = []() { fox::utils::CheckGLError(); };
#else
    auto fge = []() { /* Do nothing in release mode */ };
#endif

}  // Anonymous namespace
