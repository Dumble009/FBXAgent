#include "Model.h"

namespace fbxAgent
{
    Model::Model()
    {
        vertexPositions = std::vector<math::Vector3>();
        vertexPositonCount = 0;

        vertexIndices = std::vector<int>();
        vertexIndexCount = 0;

        vertexUVs = std::vector<std::vector<math::Vector2>>();
    }

    Model::Model(std::vector<math::Vector3> _vertexPositions, std::vector<int> _vertexIndices, std::vector<std::vector<math::Vector2>> _vertexUVs)
    {
        vertexPositions = _vertexPositions;
        vertexPositonCount = (int)vertexPositions.size();

        vertexIndices = _vertexIndices;
        vertexIndexCount = (int)vertexIndices.size();

        vertexUVs = _vertexUVs;
    }

    int Model::GetVertexPositionCount()
    {
        return vertexPositonCount;
    }

    std::vector<math::Vector3> *Model::GetVertexPositions()
    {
        return &vertexPositions;
    }

    int Model::GetVertexIndexCount()
    {
        return vertexIndexCount;
    }

    std::vector<int> *Model::GetVertexIndices()
    {
        return &vertexIndices;
    }

    std::vector<std::vector<math::Vector2>> *Model::GetVertexUVs()
    {
        return &vertexUVs;
    }
}