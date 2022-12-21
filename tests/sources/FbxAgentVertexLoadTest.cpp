#include <gtest/gtest.h>
#include "FbxAgent.h"

using namespace fbxAgent;

// 頂点座標とインデックスデータを読み込むことが出来るかどうかを確かめる
TEST(VertexLoadTest, BasicAssertions)
{
    FbxAgent agent = FbxAgent();

    auto ret = agent.Init();

    ASSERT_EQ(ret, FBX_AGENT_SUCCESS) << "ret value is : " << ret;

    ret = agent.Load("./assets/box.fbx");

    ASSERT_EQ(ret, FBX_AGENT_SUCCESS) << "ret value is : " << ret;

    Model *model;
    ret = agent.GetModelByIndex(0, &model); // 箱のfbxファイルは一つの3Dモデルのみからなる

    ASSERT_EQ(ret, FBX_AGENT_SUCCESS) << "ret value is : " << ret;

    int positionCount = model->GetVertexPositionCount();

    ASSERT_EQ(positionCount, 8);

    int indexCount = model->GetVertexIndexCount();

    ASSERT_EQ(indexCount, 6 * 2 * 3);
    /*
    int positionCount = agent.GetVertexPositionCount();

    // 箱の各角に頂点が一つずつ
    ASSERT_EQ(positionCount, 8);

    int indexCount = agent.GetVertexIndexCount();

    // 6面が2つのポリゴンがあり、1つのポリゴンに3つの頂点がある
    ASSERT_EQ(indexCount, 6 * 2 * 3);
    */
}