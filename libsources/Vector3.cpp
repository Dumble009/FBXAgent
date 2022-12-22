#include "Vector3.h"

#include "internal/common/CommonUtil.h"

namespace fbxAgent
{
    Vector3::Vector3()
    {
        x = 0;
        y = 0;
        z = 0;
    }

    Vector3::Vector3(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    bool Vector3::operator==(const Vector3 &v)
    {
        return IsFloatAlmostSame(v.x, this->x) &&
               IsFloatAlmostSame(v.y, this->y) &&
               IsFloatAlmostSame(v.z, this->z);
    }
}