#include "ModelLoader.h"

namespace fbxAgent
{
    namespace internal
    {
        FbxAgentErrorCode ModelLoader::LoadVertexPositions(const fbxsdk::FbxMesh *fbxMesh, std::vector<Vector3> *vertexPositions)
        {
            int vertexPositionCount = fbxMesh->GetControlPointsCount();

            for (int i = 0; i < vertexPositionCount; i++)
            {
                auto point = fbxMesh->GetControlPointAt(i);
                vertexPositions->push_back(
                    Vector3((float)point[0], (float)point[1], (float)point[2]));
            }

            return FbxAgentErrorCode::FBX_AGENT_SUCCESS;
        }

        FbxAgentErrorCode ModelLoader::LoadVertexIndices(const fbxsdk::FbxMesh *fbxMesh, std::vector<int> *vertexIndices)
        {
            int vertexIndexCount = fbxMesh->GetPolygonVertexCount();

            auto vertices = fbxMesh->GetPolygonVertices();

            for (int i = 0; i < vertexIndexCount; i++)
            {
                vertexIndices->push_back(vertices[i]);
            }
            return FbxAgentErrorCode::FBX_AGENT_SUCCESS;
        }

        FbxAgentErrorCode ModelLoader::LoadVertexUVs(const fbxsdk::FbxMesh *fbxMesh, std::vector<std::vector<Vector2>> *vertexUVs)
        {
            // メッシュに含まれるポリゴンの頂点の数だけUV情報を取り出す。
            // 同じメッシュ内の同一頂点は全て同じだけのレイヤを持っているという仮定を置いている。
            int vertexCount = fbxMesh->GetPolygonVertexCount();
            for (int i = 0; i < vertexCount; i++)
            {
                vertexUVs->push_back(std::vector<Vector2>());
            }

            int layerCount = fbxMesh->GetLayerCount();

            for (int layerIdx = 0; layerIdx < layerCount; layerIdx++)
            {
                // メッシュに含まれるレイヤを見ていき、UV情報を持つレイヤを見つけたらUV情報を抽出する
                auto layer = fbxMesh->GetLayer(layerIdx);
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
                    auto vertexIndices = std::vector<int>();
                    auto ret = LoadVertexIndices(fbxMesh, &vertexIndices);

                    for (auto index : vertexIndices)
                    {
                        // uvs[i]にはi番目のコントロールポイントのUV情報が含まれている。
                        // これをポリゴンの頂点毎に振りなおしたいので、インデックスバッファに含まれる各インデックスに対応するUV値を取得している
                        res.push_back(uvs[index]);
                    }
                }

                for (int i = 0; i < vertexCount; i++)
                {
                    (*vertexUVs)[i].push_back(res[i]);
                }
            }

            return FbxAgentErrorCode::FBX_AGENT_SUCCESS;
        }

        FbxAgentErrorCode ModelLoader::LoadModel(const fbxsdk::FbxMesh *fbxMesh, fbxAgent::Model *resultModel)
        {
            std::vector<Vector3> vertexPositions = std::vector<Vector3>();
            std::vector<int> vertexIndices = std::vector<int>();
            std::vector<std::vector<Vector2>> vertexUVs = std::vector<std::vector<Vector2>>();

            auto ret = LoadVertexPositions(fbxMesh, &vertexPositions);

            if (ret != FbxAgentErrorCode::FBX_AGENT_SUCCESS)
            {
                return ret;
            }

            ret = LoadVertexIndices(fbxMesh, &vertexIndices);

            if (ret != FbxAgentErrorCode::FBX_AGENT_SUCCESS)
            {
                return ret;
            }

            ret = LoadVertexUVs(fbxMesh, &vertexUVs);

            if (ret != FbxAgentErrorCode::FBX_AGENT_SUCCESS)
            {
                return ret;
            }

            ret = Triangulate(fbxMesh, &vertexIndices, &vertexUVs);

            if (ret != FbxAgentErrorCode::FBX_AGENT_SUCCESS)
            {
                return ret;
            }

            *resultModel = Model(vertexPositions, vertexIndices, vertexUVs);

            return FbxAgentErrorCode::FBX_AGENT_SUCCESS;
        }

        FbxAgentErrorCode ModelLoader::Triangulate(const fbxsdk::FbxMesh *fbxMesh, std::vector<int> *vertexIndices, std::vector<std::vector<Vector2>> *vertexUVs)
        {
            // TODO : ここはmoveに置き換えた方がいいかも
            auto org_vertexIndices = *vertexIndices;
            auto org_vertexUVs = *vertexUVs;

            vertexIndices->clear();
            vertexUVs->clear();

            int polygonCount = fbxMesh->GetPolygonCount();

            for (int i = 0; i < polygonCount; i++)
            {
                int startIndex = fbxMesh->GetPolygonVertexIndex(i);
                int vertexCount = fbxMesh->GetPolygonSize(i);

                // [0,1,2,3,4]の五角形を[0,1,2,0,2,3,0,3,4]の三角形3つに変換するような処理
                for (int j = startIndex + 1; j < startIndex + vertexCount - 1; j++)
                {
                    vertexIndices->push_back(org_vertexIndices[startIndex]);
                    vertexUVs->push_back(org_vertexUVs[startIndex]);

                    vertexIndices->push_back(org_vertexIndices[j]);
                    vertexUVs->push_back(org_vertexUVs[j]);

                    vertexIndices->push_back(org_vertexIndices[j + 1]);
                    vertexUVs->push_back(org_vertexUVs[j + 1]);
                }
            }

            return FbxAgentErrorCode::FBX_AGENT_SUCCESS;
        }
    }
}