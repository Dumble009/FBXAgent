#include <iostream>
#include "FbxAgent.h"

using namespace fbxAgent;

int main(void)
{
    std::cout << "test code start!!" << std::endl;

    FbxAgent agent = FbxAgent();

    FbxAgentErrorCode ret = 0;

    ret = agent.Init();

    if (ret < 0)
    {
        std::cerr << "agent.Init failed error code : " << ret << std::endl;
        return 1;
    }

    ret = agent.Load("./vikingroom/vikingroom.fbx");

    if (ret < 0)
    {
        std::cerr << "agent.Load failed error code : " << ret << std::endl;
        return 1;
    }

    std::cout << "test code finish!" << std::endl;

    return 0;
}