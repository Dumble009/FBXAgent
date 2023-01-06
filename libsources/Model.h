#pragma once
#include <vector>
#include "Vector/Vector2.h"
#include "Vector/Vector3.h"

namespace fbxAgent
{
    class Model
    {
    private:
        int vertexPositonCount; // 頂点座標データの数
        std::vector<math::Vector3> vertexPositions;

        int vertexIndexCount; // 頂点インデックスデータの数
        std::vector<int> vertexIndices;

        std::vector<std::vector<math::Vector2>> vertexUVs; // [vertexIndexCount][vertexUVLayerCount]の配列

    public:
        Model();
        Model(std::vector<math::Vector3> _vertexPositions,
              std::vector<int> _vertexIndices,
              std::vector<std::vector<math::Vector2>> _vertexUVs);

        int GetVertexPositionCount();
        std::vector<math::Vector3> *GetVertexPositions();

        int GetVertexIndexCount();
        std::vector<int> *GetVertexIndices();

        std::vector<std::vector<math::Vector2>> *GetVertexUVs();
    };
}