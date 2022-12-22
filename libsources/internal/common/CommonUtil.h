#pragma once

#include <float.h>
#include <math.h>

// f1とf2を比較する。epsilonは誤差の許容値
bool IsFloatAlmostSame(const float &f1, const float &f2, const float &epsilon = DBL_EPSILON);