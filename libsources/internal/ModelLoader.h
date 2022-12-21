#pragma once
#define FBXSDK_SHARED
#include <fbxsdk.h>

#include "../Model.h"
#include "../ErrorCodes.h"

namespace fbxAgent
{
    namespace internal
    {
        class ModelLoader
        {
            static FbxAgentErrorCode LoadModel(const fbxsdk::FbxMesh *fbxModel, fbxAgent::Model **resultModel);
        };
    }
}