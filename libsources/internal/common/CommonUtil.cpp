#include "CommonUtil.h"

bool IsFloatAlmostSame(const float &f1, const float &f2, const float &epsilon)
{
    return fabs(f1 - f2) < epsilon; // 2つの浮動小数点数の差分の絶対値が一定以下であれば一致しているとみなす
}