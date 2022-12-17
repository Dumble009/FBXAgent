#pragma once
#include <string>
#include <vector>
#define FBXSDK_SHARED
#include <fbxsdk.h>
#include "Vector3.h"

#define FBX_AGENT_SUCCESS (0)
#define FBX_AGENT_ERROR_FAILED_TO_CREATE_FBX_MANAGER (-1)
#define FBX_AGENT_ERROR_FAILED_TO_CREATE_FBX_IMPORETR (-2)
#define FBX_AGENT_ERROR_FAILED_TO_CREATE_FBX_SCENE (-3)
#define FBX_AGENT_ERROR_FAILED_TO_LOAD_FILE (-4)
#define FBX_AGENT_ERROR_FAILED_TO_IMPORT (-5)
#define FBX_AGENT_ERROR_AGENT_IS_NOT_INITIALIZED (-6)
#define FBX_AGENT_ERROR_FAILED_TO_LOAD_MESH_DATA (-7)

namespace fbxAgent
{
    typedef int FbxAgentErrorCode;

    class FbxAgent
    {
    private:
        fbxsdk::FbxManager *pFbxManager;
        fbxsdk::FbxImporter *pFbxImporter;
        fbxsdk::FbxScene *pFbxScene;

        int vertexPositonCount; // 頂点座標データの数
        std::vector<Vector3> vertexPositions;

        int vertexIndexCount; // 頂点インデックスデータの数
        std::vector<int> vertexIndices;

        FbxAgentErrorCode LoadVertices(const fbxsdk::FbxScene *scene);      // 頂点情報をメンバ変数にロードする
        FbxAgentErrorCode LoadVertexPositions(const fbxsdk::FbxMesh *mesh); // 各頂点の座標をメンバ変数にロードする
        FbxAgentErrorCode LoadVertexIndices(const fbxsdk::FbxMesh *mesh);   // 各頂点のインデックス情報をメンバ変数にロードする

    public:
        FbxAgent();
        ~FbxAgent();
        FbxAgentErrorCode Init(); // 初期化処理。インスタンスを使用し始める前に初期化する必要がある。
        FbxAgentErrorCode Load(std::string filePath);

        int GetVertexPositionCount();
        int GetVertexIndexCount();
    };
}