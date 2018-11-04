#ifndef __DEBUG_DRAW_H__
#define __DEBUG_DRAW_H__

#include "MathGeoLib/MathGeoLib.h"
#include "Color.h"

void DebugDraw(const AABB& aabb, Color color = White, const float4x4& transform = float4x4::identity);

#endif // __DEBUG_DRAW_H__