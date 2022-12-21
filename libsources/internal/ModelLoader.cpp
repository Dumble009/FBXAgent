#include "ModelLoader.h"

namespace fbxAgent
{
    namespace internal
    {
        FbxAgentErrorCode ModelLoader::LoadVertexPositions(const fbxsdk::FbxMesh *fbxMesh, std::vector<Vector3> *vertexPositions)
        {
            int vertexPositionCount = fbxMesh->GetControlPointsCount();

            for (int i = 0; i < vertexPositionCount; i++)
            {
                auto point = fbxMesh->GetControlPointAt(i);
                vertexPositions->push_back(
                    Vector3((float)point[0], (float)point[1], (float)point[2]));
            }

            return FbxAgentErrorCode::FBX_AGENT_SUCCESS;
        }

        FbxAgentErrorCode ModelLoader::LoadVertexIndices(const fbxsdk::FbxMesh *fbxMesh, std::vector<int> *vertexIndices)
        {
            int vertexIndexCount = fbxMesh->GetPolygonVertexCount();

            auto vertices = fbxMesh->GetPolygonVertices();

            for (int i = 0; i < vertexIndexCount; i++)
            {
                vertexIndices->push_back(vertices[i]);
            }
            return FbxAgentErrorCode::FBX_AGENT_SUCCESS;
        }

        FbxAgentErrorCode ModelLoader::LoadModel(const fbxsdk::FbxMesh *fbxMesh, fbxAgent::Model *resultModel)
        {
            std::vector<Vector3> vertexPositions = std::vector<Vector3>();
            std::vector<int> vertexIndices = std::vector<int>();
            std::vector<std::vector<Vector2>> vertexUVs = std::vector<std::vector<Vector2>>();

            LoadVertexPositions(fbxMesh, &vertexPositions);
            LoadVertexIndices(fbxMesh, &vertexIndices);

            *resultModel = Model(vertexPositions, vertexIndices, vertexUVs);

            return FbxAgentErrorCode::FBX_AGENT_SUCCESS;
        }
    }
}