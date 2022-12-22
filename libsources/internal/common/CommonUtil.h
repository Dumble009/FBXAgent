#pragma once

#include <vector>
#include <float.h>
#include <math.h>

// f1とf2を比較する。epsilonは誤差の許容値
bool IsFloatAlmostSame(const float &f1, const float &f2, const float &epsilon = DBL_EPSILON);

// Vector3のリストの中にvと一致する値が存在しているかどうか
template <class T>
bool IsValueContainedInVector(const T &v, const std::vector<T> *positions)
{ // テンプレート関数は実装と宣言を同じファイルに書かないといけない。
    return std::find(positions->begin(), positions->end(), v) != positions->end();
}