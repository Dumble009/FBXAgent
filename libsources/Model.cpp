#include "Model.h"

namespace fbxAgent
{
    Model::Model()
    {
        vertexPositions = std::vector<Vector3>();
        vertexPositonCount = 0;

        vertexIndices = std::vector<int>();
        vertexIndexCount = 0;

        vertexUVs = std::vector<std::vector<Vector2>>();
    }

    Model::Model(std::vector<Vector3> _vertexPositions, std::vector<int> _vertexIndices, std::vector<std::vector<Vector2>> _vertexUVs)
    {
        vertexPositions = _vertexPositions;
        vertexPositonCount = (int)vertexPositions.size();

        vertexIndices = _vertexIndices;
        vertexIndexCount = (int)vertexIndices.size();

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