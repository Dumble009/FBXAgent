#include "FbxAgent.h"
namespace fbxAgent
{
    FbxAgent::FbxAgent() {}

    FbxAgentErrorCode FbxAgent::Init()
    {
        pFbxManager = fbxsdk::FbxManager::Create();
        if (pFbxManager == nullptr)
        {
            return FBX_AGENT_ERROR_FAILED_TO_CREATE_FBX_MANAGER;
        }
        return FBX_AGENT_SUCCESS;
    }

    FbxAgentErrorCode FbxAgent::Load(std::string filePath)
    {
        // pFbxImporterを初期化する前にすでに存在していた場合は破棄する
        if (pFbxImporter != nullptr)
        {
            pFbxImporter->Destroy();
        }

        pFbxImporter = fbxsdk::FbxImporter::Create(pFbxManager, "importer");
        if (pFbxImporter == nullptr)
        {
            return FBX_AGENT_ERROR_FAILED_TO_CREATE_FBX_IMPORETR;
        }

        // pFbxSceneを初期化する前に存在していた場合は破棄する
        if (pFbxScene != nullptr)
        {
            pFbxScene->Destroy();
        }
        pFbxScene = fbxsdk::FbxScene::Create(pFbxManager, "scene");
        if (pFbxScene == nullptr)
        {
            return FBX_AGENT_ERROR_FAILED_TO_CREATE_FBX_SCENE;
        }

        // まずは指定されたパスからインポーターがファイルの内容を読み込む
        if (!pFbxImporter->Initialize(filePath.c_str()))
        {
            return FBX_AGENT_ERROR_FAILED_TO_LOAD_FILE;
        }

        // インポーターが読み込んだ内容を頂点情報等に分解してシーンに読み込む
        if (!pFbxImporter->Import(pFbxScene))
        {
            return FBX_AGENT_ERROR_FAILED_TO_IMPORT;
        }

        return FBX_AGENT_SUCCESS;
    }

    FbxAgent::~FbxAgent()
    {
        if (pFbxManager != nullptr)
        {
            pFbxManager->Destroy();
        }

        if (pFbxImporter != nullptr)
        {
            pFbxImporter->Destroy();
        }

        if (pFbxScene != nullptr)
        {
            pFbxScene->Destroy();
        }
    }
}