#include <iostream>
#include <chrono>

#include "FbxAgent.h"

using namespace fbxAgent;

#define FILE_NAME "./assets/vikingroom.fbx"

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

    auto start = std::chrono::system_clock::now();
    std::cout << "load start : " << FILE_NAME << std::endl;

    ret = agent.Load(FILE_NAME);

    auto end = std::chrono::system_clock::now();
    double elapsed = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0);
    std::cout << "load finish : " << elapsed << "[ms]" << std::endl;

    if (ret != FbxAgentErrorCode::FBX_AGENT_SUCCESS)
    {
        std::cerr << "agent.Load failed error code : " << (int)ret << std::endl;
        return 1;
    }

    std::cout << "test code finish!!" << std::endl;

    return 0;
}