#include "Vector2.h"

#include "internal/common/CommonUtil.h"

namespace fbxAgent
{
    Vector2::Vector2()
    {
        x = 0;
        y = 0;
    }

    Vector2::Vector2(float _x, float _y)
    {
        x = _x;
        y = _y;
    }

    bool Vector2::operator==(const Vector2 &v) const
    {
        return IsFloatAlmostSame(v.x, x) &&
               IsFloatAlmostSame(v.y, y);
    }
}