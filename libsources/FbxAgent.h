#pragma once
#include <string>
#include <vector>
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

        FbxAgentErrorCode LoadVertices(const fbxsdk::FbxScene *scene);
        FbxAgentErrorCode LoadVertexPositions(const fbxsdk::FbxMesh *mesh);
        FbxAgentErrorCode LoadVertexIndices(const fbxsdk::FbxMesh *mesh);

    public:
        FbxAgent();
        ~FbxAgent();
        FbxAgentErrorCode Init(); // 初期化処理。インスタンスを使用し始める前に初期化する必要がある。
        FbxAgentErrorCode Load(std::string filePath);
    };
}