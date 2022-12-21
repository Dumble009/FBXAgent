#pragma once
#define FBXSDK_SHARED
#include <fbxsdk.h>

#include "../Vector2.h"
#include "../Vector3.h"
#include "../Model.h"
#include "../ErrorCodes.h"

namespace fbxAgent
{
    namespace internal
    {
        class ModelLoader
        {
        private:
            static FbxAgentErrorCode LoadVertexPositions(const fbxsdk::FbxMesh *fbxMesh, std::vector<Vector3> *vertexPositions);
            static FbxAgentErrorCode LoadVertexIndices(const fbxsdk::FbxMesh *fbxMesh, std::vector<int> *vertexIndices);

        public:
            static FbxAgentErrorCode LoadModel(const fbxsdk::FbxMesh *fbxMesh, fbxAgent::Model *resultModel);
        };
    }
}