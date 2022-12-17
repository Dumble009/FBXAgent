#include "FbxAgent.h"

#define FBXSDK_NEW_API

namespace fbxAgent
{
    FbxAgent::FbxAgent()
    {
        pFbxManager = nullptr;
        pFbxImporter = nullptr;
        pFbxScene = nullptr;

        vertexPositonCount = 0;
        vertexPositions = std::vector<Vector3>();

        vertexIndexCount = 0;
        vertexIndices = std::vector<int>();
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

        auto ret = LoadVertices(pFbxScene);

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

            auto ret = LoadVertexPositions(mesh);
            if (ret != FBX_AGENT_SUCCESS)
            {
                return ret;
            }

            ret = LoadVertexIndices(mesh);
            if (ret != FBX_AGENT_SUCCESS)
            {
                return ret;
            }
        }

        return FBX_AGENT_SUCCESS;
    }

    FbxAgentErrorCode FbxAgent::LoadVertexPositions(const fbxsdk::FbxMesh *mesh)
    {
        vertexPositonCount += mesh->GetControlPointsCount();

        for (int i = 0; i < vertexPositonCount; i++)
        {
            auto point = mesh->GetControlPointAt(i);
            vertexPositions.push_back(
                Vector3((float)point[0], (float)point[1], (float)point[2]));
        }

        return FBX_AGENT_SUCCESS;
    }

    FbxAgentErrorCode FbxAgent::LoadVertexIndices(const fbxsdk::FbxMesh *mesh)
    {
        vertexIndexCount += mesh->GetPolygonVertexCount();

        auto vertices = mesh->GetPolygonVertices();

        for (int i = 0; i < vertexIndexCount; i++)
        {
            vertexIndices.push_back(vertices[i]);
        }

        return FBX_AGENT_SUCCESS;
    }

    int FbxAgent::GetVertexPositionCount()
    {
        return vertexPositonCount;
    }

    int FbxAgent::GetVertexIndexCount()
    {
        return vertexIndexCount;
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