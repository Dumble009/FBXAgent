#include <iostream>
#include <chrono>

#include "FbxAgent.h"
#include "Debugger.h"

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

    debugTool::Debugger::Start("root", debugTool::DebugType::TIME);
    std::cout << "load start : " << FILE_NAME << std::endl;

    ret = agent.Load(FILE_NAME);

    std::cout << debugTool::Debugger::Stop("root") << std::endl;

    if (ret != FbxAgentErrorCode::FBX_AGENT_SUCCESS)
    {
        std::cerr << "agent.Load failed error code : " << (int)ret << std::endl;
        return 1;
    }

    std::cout << "test code finish!!" << std::endl;

    return 0;
}