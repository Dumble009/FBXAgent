#include "FbxAgent.h"
namespace fbxAgent
{
    FbxAgent::FbxAgent()
    {
        pFbxManager = nullptr;
        pFbxImporter = nullptr;
        pFbxScene = nullptr;
    }

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
        if (pFbxManager == nullptr)
        {
            return FBX_AGENT_ERROR_AGENT_IS_NOT_INITIALIZED;
        }

        if (pFbxImporter != nullptr)
        {
            pFbxImporter->Destroy();
        }

        pFbxImporter = fbxsdk::FbxImporter::Create(pFbxManager, "importer");
        if (pFbxImporter == nullptr)
        {
            return FBX_AGENT_ERROR_FAILED_TO_CREATE_FBX_IMPORETR;
        }

        if (pFbxScene != nullptr)
        {
            pFbxScene->Destroy();
        }
        pFbxScene = fbxsdk::FbxScene::Create(pFbxManager, "scene");
        if (pFbxScene == nullptr)
        {
            return FBX_AGENT_ERROR_FAILED_TO_CREATE_FBX_SCENE;
        }

        // ファイルをImporterに読み込む
        if (!pFbxImporter->Initialize(filePath.c_str()))
        {
            return FBX_AGENT_ERROR_FAILED_TO_LOAD_FILE;
        }

        // 読み込んだファイルを頂点バッファやマテリアルなどの要素ごとに分解してSceneに展開する
        if (!pFbxImporter->Import(pFbxScene))
        {
            return FBX_AGENT_ERROR_FAILED_TO_IMPORT;
        }

        // 読み込み完了後はImporterは不要なので削除しておく
        pFbxImporter->Destroy();
        pFbxImporter = nullptr;

        // ポリゴンを全て三角ポリゴンに変換する
        FbxGeometryConverter geometryConverter(pFbxManager);
        geometryConverter.Triangulate(pFbxScene, true);

        return FBX_AGENT_SUCCESS;
    }

    FbxAgentErrorCode FbxAgent::LoadVertices(const fbxsdk::FbxScene *scene)
    {
        return FBX_AGENT_SUCCESS;
    }

    FbxAgent::~FbxAgent()
    {
        if (pFbxScene != nullptr)
        {
            pFbxScene->Destroy();
        }

        if (pFbxImporter != nullptr)
        {
            pFbxImporter->Destroy();
        }

        if (pFbxManager != nullptr)
        {
            pFbxManager->Destroy();
        }
    }
}