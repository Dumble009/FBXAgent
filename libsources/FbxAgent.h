#pragma once
#include <string>
#include <fbxsdk.h>

#define FBX_AGENT_SUCCESS (0)
#define FBX_AGENT_ERROR_FAILED_TO_CREATE_FBX_MANAGER (-1)
#define FBX_AGENT_ERROR_FAILED_TO_CREATE_FBX_IMPORETR (-2)
#define FBX_AGENT_ERROR_FAILED_TO_CREATE_FBX_SCENE (-3)
#define FBX_AGENT_ERROR_FAILED_TO_LOAD_FILE (-4)
#define FBX_AGENT_ERROR_FAILED_TO_IMPORT (-5)
#define FBX_AGENT_ERROR_AGENT_IS_NOT_INITIALIZED (-6)

namespace fbxAgent
{
    typedef int FbxAgentErrorCode;

    class FbxAgent
    {
    private:
        fbxsdk::FbxManager *pFbxManager;
        fbxsdk::FbxImporter *pFbxImporter;
        fbxsdk::FbxScene *pFbxScene;

    public:
        FbxAgent();
        ~FbxAgent();
        FbxAgentErrorCode Init();
        FbxAgentErrorCode Load(std::string filePath);
    };
}