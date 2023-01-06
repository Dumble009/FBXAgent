#pragma once
#define FBXSDK_SHARED
#include <fbxsdk.h>

#include "Vector/Vector2.h"
#include "Vector/Vector3.h"
#include "../Model.h"
#include "../ErrorCodes.h"

namespace fbxAgent
{
    namespace internal
    {
        class ModelLoader
        {
        private:
            static FbxAgentErrorCode LoadVertexPositions(const fbxsdk::FbxMesh *fbxMesh, std::vector<math::Vector3> *vertexPositions);
            static FbxAgentErrorCode LoadVertexIndices(const fbxsdk::FbxMesh *fbxMesh, std::vector<int> *vertexIndices);
            static FbxAgentErrorCode LoadVertexUVs(const fbxsdk::FbxMesh *fbxMesh, std::vector<std::vector<math::Vector2>> *vertexUVs);
            static FbxAgentErrorCode Triangulate(const fbxsdk::FbxMesh *fbxMesh, std::vector<int> *vertexIndices, std::vector<std::vector<math::Vector2>> *vertexUVs);

        public:
            static FbxAgentErrorCode LoadModel(const fbxsdk::FbxMesh *fbxMesh, fbxAgent::Model *resultModel);
        };
    }
}