#include "FoxMesh.hpp"
#include <stdexcept>

namespace fox {
    namespace utils {
        FoxMesh::FoxMesh(const std::string& modelPath) {
            tinygltf::Model model;
            fox::utils::GLTFModelData modelData;

            if (!fox::utils::LoadGLTFModel(modelPath, model, modelData)) {
                throw std::runtime_error("Failed to load GLTF model");
            }

            numVertices = modelData.posAccessor.count;
            std::vector<float> positionVec(modelData.positionBuffer, modelData.positionBuffer + numVertices * 3);
            std::vector<float> normalVec(modelData.normalBuffer, modelData.normalBuffer + numVertices * 3);
            std::vector<float> texCoordVec(modelData.texCoordBuffer, modelData.texCoordBuffer + numVertices * 2);
            std::vector<float> interleavedData = fox::utils::interleaveData(positionVec, normalVec, texCoordVec, 8);

            setupMesh(interleavedData);
        }

        FoxMesh::~FoxMesh() {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
        }

        void FoxMesh::setupMesh(const std::vector<float>& interleavedData) {
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
        }

        void FoxMesh::draw() const {
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(numVertices));
            glBindVertexArray(0);
        }

        void FoxMesh::destroy() const {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
        }
    }
}


