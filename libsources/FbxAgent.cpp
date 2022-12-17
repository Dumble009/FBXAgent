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
        auto mesh = scene->GetSrcObject<fbxsdk::FbxMesh>();
        if (mesh == nullptr)
        {
            return FBX_AGENT_ERROR_FAILED_TO_LOAD_MESH_DATA;
        }

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

        return FBX_AGENT_SUCCESS;
    }

    FbxAgentErrorCode FbxAgent::LoadVertexPositions(const fbxsdk::FbxMesh *mesh)
    {
        vertexPositonCount = mesh->GetControlPointsCount();
        vertexPositions = std::vector<Vector3>(vertexPositonCount);

        for (int i = 0; i < vertexPositonCount; i++)
        {
            auto point = mesh->GetControlPointAt(i);
            vertexPositions[i].x = (float)point[0];
            vertexPositions[i].y = (float)point[1];
            vertexPositions[i].z = (float)point[2];
        }

        return FBX_AGENT_SUCCESS;
    }

    FbxAgentErrorCode FbxAgent::LoadVertexIndices(const fbxsdk::FbxMesh *mesh)
    {
        vertexIndexCount = mesh->GetPolygonVertexCount();
        vertexIndices = std::vector<int>(vertexIndexCount);

        auto vertices = mesh->GetPolygonVertices();

        for (int i = 0; i < vertexIndexCount; i++)
        {
            vertexIndices[i] = vertices[i];
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