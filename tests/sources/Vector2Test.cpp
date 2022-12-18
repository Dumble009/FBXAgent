#include <gtest/gtest.h>
#include "Vector2.h"

// Vector2クラスの挙動をテストする

// 引数なしのコンストラクタがx,y全てが0のベクトルを作るかどうか
TEST(Vector2DefaultInit, BasicAssertions)
{
    fbxAgent::Vector2 vec = fbxAgent::Vector2();
    EXPECT_FLOAT_EQ(vec.x, 0.0f);
    EXPECT_FLOAT_EQ(vec.y, 0.0f);
}

// 引数付きのコンストラクタがちゃんとその値をx,y,zに代入したベクトルを作るかどうか
TEST(Vector2ExplicitInit, BasicAssertions)
{
    float x = 1.0f;
    float y = 1000000.5f;
    fbxAgent::Vector2 vec = fbxAgent::Vector2(x, y);
    EXPECT_FLOAT_EQ(vec.x, x);
    EXPECT_FLOAT_EQ(vec.y, y);
}