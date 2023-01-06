#pragma once
#include <string>
#include <vector>
#include <queue>

#define FBXSDK_SHARED
#include <fbxsdk.h>
#include "Model.h"
#include "ErrorCodes.h"

namespace fbxAgent
{
    class FbxAgent
    {
    private:
        fbxsdk::FbxManager *pFbxManager;
        fbxsdk::FbxImporter *pFbxImporter;
        fbxsdk::FbxScene *pFbxScene;

        std::vector<Model> models; // fbxファイルから抽出したモデルのリスト

        FbxAgentErrorCode LoadVertices(const fbxsdk::FbxScene *scene);     // 頂点情報をメンバ変数にロードする
        FbxAgentErrorCode LoadVertexPosition(const fbxsdk::FbxMesh *mesh); // 各頂点の座標をメンバ変数にロードする
        FbxAgentErrorCode LoadVertexIndices(const fbxsdk::FbxMesh *mesh);  // 各頂点のインデックス情報をメンバ変数にロードする
        FbxAgentErrorCode LoadVertexUVs(const fbxsdk::FbxMesh *mesh);      // UV情報を取り出す。UV情報は各ポリゴンの頂点毎に割り振られる。

    public:
        FbxAgent();
        ~FbxAgent();
        FbxAgentErrorCode Init(); // 初期化処理。インスタンスを使用し始める前に初期化する必要がある。
        FbxAgentErrorCode Load(const std::string &filePath);

        int GetVertexPositionCount();
        int GetVertexIndexCount();

        FbxAgentErrorCode GetModelByIndex(int index, Model **model);
    };
}