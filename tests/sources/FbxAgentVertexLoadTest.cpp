#include <gtest/gtest.h>

#include <algorithm>
#include "FbxAgent.h"

#include "internal/common/CommonUtil.h"

#define IsVector3ContainedInVector(x, y, z, v) ASSERT_EQ(IsValueContainedInVector(Vector3(x, y, z), v), true)
#define IsVector2ContainedInVector(x, y, v) ASSERT_EQ(IsValueContainedInVector(Vector2(x, y), v), true)

using namespace fbxAgent;

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
    auto firstLayer = std::vector<Vector2>();

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

// 頂点座標とインデックスデータを読み込むことが出来るかどうかを確かめる
TEST(VertexLoadTest, BasicAssertions)
{
    LoadTestBody("./assets/tri_box.fbx");
}