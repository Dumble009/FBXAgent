#include <gtest/gtest.h>
#include "FbxAgent.h"

using namespace fbxAgent;

// 頂点座標とインデックスデータを読み込むことが出来るかどうかを確かめる
TEST(VertexLoadTest, BasicAssertions)
{
    FbxAgent agent = FbxAgent();

    auto ret = agent.Init();

    ASSERT_EQ(ret, FBX_AGENT_SUCCESS) << "ret value is : " << ret;

    ret = agent.Load("./vikingroom/vikingroom.fbx");

    ASSERT_EQ(ret, FBX_AGENT_SUCCESS) << "ret value is : " << ret;

    int positionCount = agent.GetVertexPositionCount();

    ASSERT_EQ(positionCount, 6179);

    int indexCount = agent.GetVertexIndexCount();

    ASSERT_EQ(indexCount, 3829 * 3);
}