#include <iostream>
#include "FbxAgent.h"

using namespace fbxAgent;

int main(void)
{
    std::cout << "test code start!!" << std::endl;

    FbxAgent agent = FbxAgent();

    FbxAgentErrorCode ret = FbxAgentErrorCode::FBX_AGENT_SUCCESS;

    ret = agent.Init();

    std::cout << "initialize finished!!" << std::endl;

    if (ret != FbxAgentErrorCode::FBX_AGENT_SUCCESS)
    {
        std::cerr << "agent.Init failed error code : " << (int)ret << std::endl;
        return 1;
    }

    ret = agent.Load("./assets/vikingroom.fbx");

    if (ret != FbxAgentErrorCode::FBX_AGENT_SUCCESS)
    {
        std::cerr << "agent.Load failed error code : " << (int)ret << std::endl;
        return 1;
    }

    std::cout << "test code finish!!" << std::endl;

    return 0;
}