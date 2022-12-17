#include <gtest/gtest.h>
#include "Vector3.h"

// Vector3クラスの挙動をテストする

// 引数なしのコンストラクタがx,y,z全てが0のベクトルを作るかどうか
TEST(Vector3DefaultInit, BasicAssertions)
{
    fbxAgent::Vector3 vec = fbxAgent::Vector3();
    EXPECT_FLOAT_EQ(vec.x, 0.0f);
    EXPECT_FLOAT_EQ(vec.y, 0.0f);
    EXPECT_FLOAT_EQ(vec.z, 0.0f);
}

// 引数付きのコンストラクタがちゃんとその値をx,y,zに代入したベクトルを作るかどうか
TEST(Vector3ExplicitInit, BasicAssertions)
{
    float x = 1.0f;
    float y = 1.5f;
    float z = 1000000.555f;
    fbxAgent::Vector3 vec = fbxAgent::Vector3(x, y, z);
    EXPECT_FLOAT_EQ(vec.x, x);
    EXPECT_FLOAT_EQ(vec.y, y);
    EXPECT_FLOAT_EQ(vec.z, z);
}