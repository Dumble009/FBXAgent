#include <gtest/gtest.h>

#include <algorithm>
#include <vector>
#include "FbxAgent.h"

#include "Vector/Vector2.h"
#include "Vector/Vector3.h"

template <class T>
bool IsValueContainedInVector(const T &val, const std::vector<T> *v)
{
    return v->end() != std::find(v->begin(), v->end(), val);
}

#define IsVector3ContainedInVector(x, y, z, v) ASSERT_EQ(IsValueContainedInVector(math::Vector3(x, y, z), v), true)
#define IsVector2ContainedInVector(x, y, v) ASSERT_EQ(IsValueContainedInVector(math::Vector2(x, y), v), true)

using namespace fbxAgent;

// 立方体の3Dモデルを読み込んで、頂点の数やUVが合っているかを確認する
void LoadTestBody(const std::string &fileName)
{
    FbxAgent agent = FbxAgent();

    auto ret = agent.Init();

    ASSERT_EQ(ret, fbxAgent::FbxAgentErrorCode::FBX_AGENT_SUCCESS) << "ret value is : " << (int)ret;

    ret = agent.Load(fileName);

    ASSERT_EQ(ret, fbxAgent::FbxAgentErrorCode::FBX_AGENT_SUCCESS) << "ret value is : " << (int)ret;

    Model *model;
    ret = agent.GetModelByIndex(0, &model); // 箱のfbxファイルは一つの3Dモデルのみからなる

    ASSERT_EQ(ret, fbxAgent::FbxAgentErrorCode::FBX_AGENT_SUCCESS) << "ret value is : " << (int)ret;

    int positionCount = model->GetVertexPositionCount();

    ASSERT_EQ(positionCount, 8);

    auto vertexPositions = model->GetVertexPositions();

    int indexCount = model->GetVertexIndexCount();

    ASSERT_EQ(indexCount, 6 * 2 * 3);

    // 各コントロールポイントの座標が正しいかどうかの判定
    IsVector3ContainedInVector(1, 1, 1, vertexPositions);
    IsVector3ContainedInVector(-1, 1, 1, vertexPositions);
    IsVector3ContainedInVector(1, -1, 1, vertexPositions);
    IsVector3ContainedInVector(-1, -1, 1, vertexPositions);
    IsVector3ContainedInVector(1, 1, -1, vertexPositions);
    IsVector3ContainedInVector(-1, 1, -1, vertexPositions);
    IsVector3ContainedInVector(1, -1, -1, vertexPositions);
    IsVector3ContainedInVector(-1, -1, -1, vertexPositions);

    auto vertexUVs = model->GetVertexUVs();
    auto firstLayer = std::vector<math::Vector2>();

    for (auto itr = vertexUVs->begin(); itr != vertexUVs->end(); itr++)
    {
        firstLayer.push_back(itr->at(0));
    }

    // UV座標が正しいかどうかの判定
    float block = 1.0f / 8.0f; // UV確認用のテクスチャの1ブロックの大きさ
    IsVector2ContainedInVector(5.0f * block, 6.0f * block, &firstLayer);
    IsVector2ContainedInVector(7.0f * block, 6.0f * block, &firstLayer);
    IsVector2ContainedInVector(7.0f * block, 4.0f * block, &firstLayer);
    IsVector2ContainedInVector(5.0f * block, 4.0f * block, &firstLayer);

    IsVector2ContainedInVector(3.0f * block, 6.0f * block, &firstLayer);
    IsVector2ContainedInVector(5.0f * block, 6.0f * block, &firstLayer);
    IsVector2ContainedInVector(5.0f * block, 4.0f * block, &firstLayer);
    IsVector2ContainedInVector(3.0f * block, 4.0f * block, &firstLayer);

    IsVector2ContainedInVector(1.0f * block, 6.0f * block, &firstLayer);
    IsVector2ContainedInVector(3.0f * block, 6.0f * block, &firstLayer);
    IsVector2ContainedInVector(3.0f * block, 4.0f * block, &firstLayer);
    IsVector2ContainedInVector(1.0f * block, 4.0f * block, &firstLayer);

    IsVector2ContainedInVector(3.0f * block, 8.0f * block, &firstLayer);
    IsVector2ContainedInVector(5.0f * block, 8.0f * block, &firstLayer);
    IsVector2ContainedInVector(5.0f * block, 6.0f * block, &firstLayer);
    IsVector2ContainedInVector(3.0f * block, 6.0f * block, &firstLayer);

    IsVector2ContainedInVector(3.0f * block, 4.0f * block, &firstLayer);
    IsVector2ContainedInVector(5.0f * block, 4.0f * block, &firstLayer);
    IsVector2ContainedInVector(5.0f * block, 2.0f * block, &firstLayer);
    IsVector2ContainedInVector(3.0f * block, 2.0f * block, &firstLayer);

    IsVector2ContainedInVector(3.0f * block, 2.0f * block, &firstLayer);
    IsVector2ContainedInVector(5.0f * block, 2.0f * block, &firstLayer);
    IsVector2ContainedInVector(5.0f * block, 0.0f * block, &firstLayer);
    IsVector2ContainedInVector(3.0f * block, 0.0f * block, &firstLayer);
}

// 全ての面が三角形のモデルの頂点座標とインデックスデータを読み込むことが出来るかどうかを確かめる
TEST(VertexLoadTest_Tri, BasicAssertions)
{
    LoadTestBody("./assets/tri_box.fbx");
}

// 全ての面が四角形のモデルについても同様の事を確かめる
TEST(VertexLoadTest_Quad, BasicAssertions)
{
    LoadTestBody("./assets/quad_box.fbx");
}