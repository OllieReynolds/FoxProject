// Mesh.hpp
#pragma once
#include <string>
#include <vector>
#include "FoxUtils.hpp"

namespace fox {
    namespace utils {
        class FoxMesh {
        public:
            FoxMesh(const std::string& modelPath);
            ~FoxMesh();

            void draw() const;
            void destroy() const;

        private:
            unsigned int VAO, VBO;
            size_t numVertices;

            void setupMesh(const std::vector<float>& interleavedData);
        };
    }
}


