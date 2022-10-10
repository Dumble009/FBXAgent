﻿#include "FbxAgent.h"
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

        if (!pFbxImporter->Initialize(filePath.c_str()))
        {
            return FBX_AGENT_ERROR_FAILED_TO_LOAD_FILE;
        }

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