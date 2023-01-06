#include "FbxAgent.h"

// internelなヘッダはユーザに見せないので、実装部でインクルードする
#include "internal/ModelLoader.h"

#include "Debugger.h"
#include <iostream>

#define FBXSDK_NEW_API

// #define DEBUGGER_ON

#ifdef DEBUGGER_ON
#define DEBUG_START(name, type) debugTool::Debugger::Start(name, type)

#define DEBUG_STOP(name) std::cout << debugTool::Debugger::Stop(name) << std::endl
#else
#define DEBUG_START(name, type)

#define DEBUG_STOP(name)
#endif

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
            return FbxAgentErrorCode::FBX_AGENT_ERROR_FAILED_TO_CREATE_FBX_MANAGER;
        }
        return FbxAgentErrorCode::FBX_AGENT_SUCCESS;
    }

    FbxAgentErrorCode FbxAgent::Load(const std::string &filePath)
    {
        if (pFbxManager == nullptr)
        {
            return FbxAgentErrorCode::FBX_AGENT_ERROR_AGENT_IS_NOT_INITIALIZED;
        }

        if (pFbxImporter != nullptr)
        {
            pFbxImporter->Destroy();
        }

        DEBUG_START("create importer", debugTool::DebugType::TIME);
        pFbxImporter = fbxsdk::FbxImporter::Create(pFbxManager, "importer");
        if (pFbxImporter == nullptr)
        {
            return FbxAgentErrorCode::FBX_AGENT_ERROR_FAILED_TO_CREATE_FBX_IMPORETR;
        }
        DEBUG_STOP("create importer");

        if (pFbxScene != nullptr)
        {
            pFbxScene->Destroy();
        }

        DEBUG_START("create scene", debugTool::DebugType::TIME);
        pFbxScene = fbxsdk::FbxScene::Create(pFbxManager, "scene");
        if (pFbxScene == nullptr)
        {
            return FbxAgentErrorCode::FBX_AGENT_ERROR_FAILED_TO_CREATE_FBX_SCENE;
        }
        DEBUG_STOP("create scene");

        DEBUG_START("file load", debugTool::DebugType::TIME);
        // ファイルをImporterに読み込む
        if (!pFbxImporter->Initialize(filePath.c_str()))
        {
            return FbxAgentErrorCode::FBX_AGENT_ERROR_FAILED_TO_LOAD_FILE;
        }
        DEBUG_STOP("file load");

        DEBUG_START("import", debugTool::DebugType::TIME);
        // 読み込んだファイルを頂点バッファやマテリアルなどの要素ごとに分解してSceneに展開する
        if (!pFbxImporter->Import(pFbxScene))
        {
            return FbxAgentErrorCode::FBX_AGENT_ERROR_FAILED_TO_IMPORT;
        }
        DEBUG_STOP("import");

        // 読み込み完了後はImporterは不要なので削除しておく
        pFbxImporter->Destroy();
        pFbxImporter = nullptr;

        DEBUG_START("Triangulate", debugTool::DebugType::TIME);
        // ポリゴンを全て三角ポリゴンに変換する
        fbxsdk::FbxGeometryConverter geometryConverter(pFbxManager);
        // geometryConverter.Triangulate(pFbxScene, true);
        DEBUG_STOP("Triangulate");

        DEBUG_START("LoadVertices", debugTool::DebugType::TIME);
        auto ret = LoadVertices(pFbxScene);
        DEBUG_STOP("LoadVertices");

        return ret;
    }

    FbxAgentErrorCode FbxAgent::LoadVertices(const fbxsdk::FbxScene *scene)
    {
        // sceneに含まれる全てのノードを探索し、
        // 各ノードに含まれる全てのAttributeを調べて、その中からMeshを全て取り出す
        std::queue<fbxsdk::FbxMesh *> meshQ;
        std::queue<fbxsdk::FbxNode *> nodeQ;

        auto rootNode = scene->GetRootNode();
        nodeQ.push(rootNode);
        while (!nodeQ.empty())
        {
            auto node = nodeQ.front();
            nodeQ.pop();

            int attributeCount = node->GetNodeAttributeCount();
            for (int i = 0; i < attributeCount; i++)
            {
                auto attribute = node->GetNodeAttributeByIndex(i);
                auto type = attribute->GetAttributeType();

                if (type == fbxsdk::FbxNodeAttribute::EType::eMesh)
                {
                    meshQ.push((fbxsdk::FbxMesh *)attribute);
                }
            }

            int childCount = node->GetChildCount();
            for (int i = 0; i < childCount; i++)
            {
                auto child = node->GetChild(i);
                nodeQ.push(child);
            }
        }

        while (!meshQ.empty())
        {
            auto mesh = meshQ.front();
            meshQ.pop();

            models.push_back(Model());

            internal::ModelLoader::LoadModel(mesh, &(models.back()));
        }

        return FbxAgentErrorCode::FBX_AGENT_SUCCESS;
    }

    FbxAgentErrorCode FbxAgent::GetModelByIndex(int index, Model **model)
    {
        if (index >= (int)models.size())
        {
            return FbxAgentErrorCode::FBX_AGENT_ERROR_MODEL_INDEX_OUT_OF_RANGE;
        }

        *model = &(models[index]);

        return FbxAgentErrorCode::FBX_AGENT_SUCCESS;
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