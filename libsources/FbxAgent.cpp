#include "FbxAgent.h"

// internelなヘッダはユーザに見せないので、実装部でインクルードする
#include "internal/ModelLoader.h"

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

        vertexUVs = std::vector<std::vector<Vector2>>();
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

    FbxAgentErrorCode FbxAgent::Load(std::string filePath)
    {
        if (pFbxManager == nullptr)
        {
            return FbxAgentErrorCode::FBX_AGENT_ERROR_AGENT_IS_NOT_INITIALIZED;
        }

        if (pFbxImporter != nullptr)
        {
            pFbxImporter->Destroy();
        }

        pFbxImporter = fbxsdk::FbxImporter::Create(pFbxManager, "importer");
        if (pFbxImporter == nullptr)
        {
            return FbxAgentErrorCode::FBX_AGENT_ERROR_FAILED_TO_CREATE_FBX_IMPORETR;
        }

        if (pFbxScene != nullptr)
        {
            pFbxScene->Destroy();
        }
        pFbxScene = fbxsdk::FbxScene::Create(pFbxManager, "scene");
        if (pFbxScene == nullptr)
        {
            return FbxAgentErrorCode::FBX_AGENT_ERROR_FAILED_TO_CREATE_FBX_SCENE;
        }

        // ファイルをImporterに読み込む
        if (!pFbxImporter->Initialize(filePath.c_str()))
        {
            return FbxAgentErrorCode::FBX_AGENT_ERROR_FAILED_TO_LOAD_FILE;
        }

        // 読み込んだファイルを頂点バッファやマテリアルなどの要素ごとに分解してSceneに展開する
        if (!pFbxImporter->Import(pFbxScene))
        {
            return FbxAgentErrorCode::FBX_AGENT_ERROR_FAILED_TO_IMPORT;
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

            models.push_back(Model());

            internal::ModelLoader::LoadModel(mesh, &(models.back()));

            /*auto ret = LoadVertexPosition(mesh);
if (ret != FbxAgentErrorCode::FBX_AGENT_SUCCESS)
{
    return ret;
}

ret = LoadVertexIndices(mesh);
if (ret != FbxAgentErrorCode::FBX_AGENT_SUCCESS)
{
    return ret;
}*/
        }

        return FbxAgentErrorCode::FBX_AGENT_SUCCESS;
    }

    FbxAgentErrorCode FbxAgent::LoadVertexPosition(const fbxsdk::FbxMesh *mesh)
    {
        vertexPositonCount += mesh->GetControlPointsCount();

        for (int i = 0; i < vertexPositonCount; i++)
        {
            auto point = mesh->GetControlPointAt(i);
            vertexPositions.push_back(
                Vector3((float)point[0], (float)point[1], (float)point[2]));
        }

        return FbxAgentErrorCode::FBX_AGENT_SUCCESS;
    }

    FbxAgentErrorCode FbxAgent::LoadVertexIndices(const fbxsdk::FbxMesh *mesh)
    {
        vertexIndexCount += mesh->GetPolygonVertexCount();

        auto vertices = mesh->GetPolygonVertices();

        for (int i = 0; i < vertexIndexCount; i++)
        {
            vertexIndices.push_back(vertices[i]);
        }

        return FbxAgentErrorCode::FBX_AGENT_SUCCESS;
    }

    // LoadVertexIndicesの後に呼び出されることを前提としている
    FbxAgentErrorCode FbxAgent::LoadVertexUVs(const fbxsdk::FbxMesh *mesh)
    {
        // TODO : UV座標をどういう風に展開すればVulkanで扱いやすい形に出来るのか考える。マッピングモードの違いにどう対処する？
        // 同じFbxMesh内の頂点は全て同じレイヤを持っているという前提を置いている
        int layerCount = mesh->GetLayerCount();
        int startIndex = vertexUVs.size();
        int vertexCount = mesh->GetPolygonVertexCount();

        for (int i = 0; i < vertexCount; i++)
        {
            vertexUVs.push_back(std::vector<Vector2>());
        }

        for (int layerIdx = 0; layerIdx < layerCount; layerIdx++)
        {
            auto layer = mesh->GetLayer(layerIdx);
            auto elem = layer->GetUVs();
            if (elem == nullptr)
            {
                continue;
            }

            int uvArraySize = elem->GetDirectArray().GetCount();
            int indexSize = elem->GetIndexArray().GetCount();
            int size = std::max(uvArraySize, indexSize); // マッピングの仕方によってUV情報の数が異なる

            std::vector<Vector2> uvs = std::vector<Vector2>();

            auto refMode = elem->GetReferenceMode();

            for (int i = 0; i < size; i++)
            {
                fbxsdk::FbxVector2 uv;
                // UV情報の参照の仕方には、直接アクセスするものとインデックスバッファを使用するものがある
                // ここでいうインデックスは頂点インデックスとは別物
                if (refMode == fbxsdk::FbxLayerElement::eDirect)
                {
                    uv = elem->GetDirectArray().GetAt(i);
                }
                else
                {
                    int index = elem->GetIndexArray().GetAt(i);
                    uv = elem->GetDirectArray().GetAt(index);
                }

                float x = (float)uv[0];
                float y = (float)uv[1];
                uvs.emplace_back(x, y);
            }

            std::vector<Vector2> res;

            auto mapMode = elem->GetMappingMode();
            if (mapMode == fbxsdk::FbxLayerElement::EMappingMode::eByPolygonVertex)
            {
                res = uvs; // uv情報がポリゴンの頂点毎に割り当てられている場合はそのまま結果として使える。
            }
            else
            {
                // uv情報がコントロールポイント毎に割り当てられている場合は、ポリゴンの頂点毎に振りなおす必要がある
                for (int i = startIndex; i < startIndex + vertexCount; i++)
                {
                    int vertexIndex = vertexIndices[i]; //
                }
            }
        }

        return FbxAgentErrorCode::FBX_AGENT_SUCCESS;
    }

    int FbxAgent::GetVertexPositionCount()
    {
        return vertexPositonCount;
    }

    int FbxAgent::GetVertexIndexCount()
    {
        return vertexIndexCount;
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