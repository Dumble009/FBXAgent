#include "Model.h"

namespace fbxAgent
{
    Model::Model()
    {
        vertexPositions = std::vector<Vector3>();
        vertexIndices = std::vector<int>();
        vertexUVs = std::vector<std::vector<Vector2>>();
    }

    Model::Model(std::vector<Vector3> _vertexPositions, std::vector<int> _vertexIndices, std::vector<std::vector<Vector2>> _vertexUVs)
    {
        vertexPositions = _vertexPositions;
        vertexIndices = _vertexIndices;
        vertexUVs = _vertexUVs;
    }

    int Model::GetVertexPositionCount()
    {
        return 0;
    }

    std::vector<Vector3> *Model::GetVertexPositions()
    {
        return nullptr;
    }

    int Model::GetVertexIndexCount()
    {
        return 0;
    }

    std::vector<int> *Model::GetVertexIndices()
    {
        return nullptr;
    }

    std::vector<std::vector<Vector2>> *Model::GetVertexUVs()
    {
        return nullptr;
    }
}