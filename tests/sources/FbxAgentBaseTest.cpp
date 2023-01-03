#include <gtest/gtest.h>
#include "FbxAgent.h"

// FbxAgentの初期化やモデルをロードできるか等基本的な項目をテストする

// FbxAgent::Initを実行してもエラーが返されないかを確認する
TEST(FbxAgentInit, BasicAssertions)
{
    fbxAgent::FbxAgent agent = fbxAgent::FbxAgent();

    auto ret = agent.Init();

    ASSERT_EQ(ret, fbxAgent::FbxAgentErrorCode::FBX_AGENT_SUCCESS) << "ret value is : " << (int)ret;
}

TEST(FbxAgentLoad, BasicAssertions)
{
    fbxAgent::FbxAgent agent = fbxAgent::FbxAgent();

    agent.Init();

    auto ret = agent.Load("./assets/tri_box.fbx");

    ASSERT_EQ(ret, fbxAgent::FbxAgentErrorCode::FBX_AGENT_SUCCESS) << "ret value is : " << (int)ret;
}