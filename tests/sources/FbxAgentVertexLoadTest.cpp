#include <gtest/gtest.h>
#include "FbxAgent.h"

using namespace fbxAgent;

// Vector3のリストの中にvと一致するベクトルが存在しているかどうか
bool IsPositionContainedInVector(const Vector3 &v, const std::vector<Vector3> *positions)
{
    for (auto itr = positions->begin(); itr != positions->end(); itr++)
    {
        if ((*itr) == v)
        {
            return true;
        }
    }
    return false;
}

// 頂点座標とインデックスデータを読み込むことが出来るかどうかを確かめる
TEST(VertexLoadTest, BasicAssertions)
{
    FbxAgent agent = FbxAgent();

    auto ret = agent.Init();

    ASSERT_EQ(ret, fbxAgent::FbxAgentErrorCode::FBX_AGENT_SUCCESS) << "ret value is : " << (int)ret;

    ret = agent.Load("./assets/box.fbx");

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
    ASSERT_EQ(IsPositionContainedInVector(Vector3(1, 1, 1), model->GetVertexPositions()), true);
    ASSERT_EQ(IsPositionContainedInVector(Vector3(-1, 1, 1), model->GetVertexPositions()), true);
    ASSERT_EQ(IsPositionContainedInVector(Vector3(1, -1, 1), model->GetVertexPositions()), true);
    ASSERT_EQ(IsPositionContainedInVector(Vector3(-1, -1, 1), model->GetVertexPositions()), true);
    ASSERT_EQ(IsPositionContainedInVector(Vector3(1, 1, -1), model->GetVertexPositions()), true);
    ASSERT_EQ(IsPositionContainedInVector(Vector3(-1, 1, -1), model->GetVertexPositions()), true);
    ASSERT_EQ(IsPositionContainedInVector(Vector3(1, -1, -1), model->GetVertexPositions()), true);
    ASSERT_EQ(IsPositionContainedInVector(Vector3(-1, -1, -1), model->GetVertexPositions()), true);
}