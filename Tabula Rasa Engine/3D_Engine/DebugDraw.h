#ifndef __DEBUG_DRAW_H__
#define __DEBUG_DRAW_H__

#include "MathGeoLib/MathGeoLib.h"
#include "Color.h"

void DebugDraw(const AABB& aabb, Color color = White, const float4x4& transform = float4x4::identity);
void DebugDraw(const Frustum& frustum, Color color = White, const float4x4& transform = float4x4::identity);
void DebugDraw(const LineSegment& line_segment, Color color = White, const float4x4& transform = float4x4::identity);
void DebugDraw(const float3 pos, Color color = White, const float4x4 & transform = float4x4::identity);

#endif // __DEBUG_DRAW_H__