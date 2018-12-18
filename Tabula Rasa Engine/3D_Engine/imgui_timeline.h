#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "MathGeoLib\MathGeoLib.h"


static const float NODE_SLOT_RADIUS = 4.0f;


namespace ImGui
{
	static float s_max_timeline_value;


	bool BeginTimeline(const char* str_id, float max_value)
	{
		s_max_timeline_value = max_value;
		return BeginChild(str_id);
	}


	static const float TIMELINE_RADIUS = 6;


	bool TimelineEvent(const char* str_id, float* values)
	{
		ImGuiWindow* win = GetCurrentWindow();
		const ImU32 inactive_color = ColorConvertFloat4ToU32(GImGui->Style.Colors[ImGuiCol_Button]);
		const ImU32 active_color = ColorConvertFloat4ToU32(GImGui->Style.Colors[ImGuiCol_ButtonHovered]);
		const ImU32 line_color = ColorConvertFloat4ToU32(GImGui->Style.Colors[ImGuiCol_ColumnActive]);
		bool changed = false;
		ImVec2 cursor_pos = win->DC.CursorPos;

		for (int i = 0; i < 2; ++i)
		{
			ImVec2 pos = cursor_pos;
			pos.x += win->Size.x * values[i] / s_max_timeline_value + TIMELINE_RADIUS;
			pos.y += TIMELINE_RADIUS;

			float2 tmp_pos(pos.x, pos.y);
			float2 tmp_time_line_radius(TIMELINE_RADIUS, TIMELINE_RADIUS);
			float2 total_pos_rad = tmp_pos + tmp_time_line_radius;
			ImVec2 total_pos_rad_vec2(total_pos_rad.x, total_pos_rad.y);
			SetCursorScreenPos(total_pos_rad_vec2);
			PushID(i);
			InvisibleButton(str_id, ImVec2(2 * TIMELINE_RADIUS, 2 * TIMELINE_RADIUS));
			if (IsItemActive() || IsItemHovered())
			{
				ImGui::SetTooltip("%f", values[i]);
				ImVec2 a(pos.x, GetWindowContentRegionMin().y + win->Pos.y);
				ImVec2 b(pos.x, GetWindowContentRegionMax().y + win->Pos.y);
				win->DrawList->AddLine(a, b, line_color);
			}
			if (IsItemActive() && IsMouseDragging(0))
			{
				values[i] += GetIO().MouseDelta.x / win->Size.x * s_max_timeline_value;
				changed = true;
			}
			PopID();
			win->DrawList->AddCircleFilled(
				pos, TIMELINE_RADIUS, IsItemActive() || IsItemHovered() ? active_color : inactive_color);
		}

		ImVec2 start = cursor_pos;
		start.x += win->Size.x * values[0] / s_max_timeline_value + 2 * TIMELINE_RADIUS;
		start.y += TIMELINE_RADIUS * 0.5f;

		float2 tmp_pos(start.x, start.y);
		float2 tmp_time_line_radius(win->Size.x * (values[1] - values[0]) / s_max_timeline_value - 2 * TIMELINE_RADIUS, TIMELINE_RADIUS);
		float2 total_pos_rad = tmp_pos + tmp_time_line_radius;
		ImVec2 total_pos_rad_vec2(total_pos_rad.x, total_pos_rad.y);

		ImVec2 end = total_pos_rad_vec2;
		float2 real_end = total_pos_rad - tmp_pos;
		ImVec2 real_real_end(real_end.x,real_end.y);
		PushID(-1);
		SetCursorScreenPos(start);

		InvisibleButton(str_id, real_real_end);
		if (IsItemActive() && IsMouseDragging(0))
		{
			values[0] += GetIO().MouseDelta.x / win->Size.x * s_max_timeline_value;
			values[1] += GetIO().MouseDelta.x / win->Size.x * s_max_timeline_value;
			changed = true;
		}
		PopID();

		float2 tmp_pos2(cursor_pos.x, cursor_pos.y);
		float2 tmp_time_line_radius2(0, GetTextLineHeightWithSpacing());
		float2 total_pos_rad2 = tmp_pos2 + tmp_time_line_radius2;
		ImVec2 total_pos_rad_vec22(total_pos_rad2.x, total_pos_rad2.y);

		SetCursorScreenPos(total_pos_rad_vec22);

		win->DrawList->AddRectFilled(start, end, IsItemActive() || IsItemHovered() ? active_color : inactive_color);

		if (values[0] > values[1])
		{
			float tmp = values[0];
			values[0] = values[1];
			values[1] = tmp;
		}
		if (values[1] > s_max_timeline_value) values[1] = s_max_timeline_value;
		if (values[0] < 0) values[0] = 0;
		return changed;
	}


	void EndTimeline()
	{
		ImGuiWindow* win = GetCurrentWindow();

		ImU32 color = ColorConvertFloat4ToU32(GImGui->Style.Colors[ImGuiCol_Button]);
		ImU32 line_color = ColorConvertFloat4ToU32(GImGui->Style.Colors[ImGuiCol_Border]);
		ImU32 text_color = ColorConvertFloat4ToU32(GImGui->Style.Colors[ImGuiCol_Text]);
		float rounding = GImGui->Style.ScrollbarRounding;
		ImVec2 start(GetWindowContentRegionMin().x + win->Pos.x,
			GetWindowContentRegionMax().y - GetTextLineHeightWithSpacing() + win->Pos.y);

		float2 tmp_pos2(GetWindowContentRegionMax().x, GetWindowContentRegionMax().y);
		float2 tmp_time_line_radius2(win->Pos.x, win->Pos.y);
		float2 total_pos_rad2 = tmp_pos2 + tmp_time_line_radius2;
		ImVec2 total_pos_rad_vec22(total_pos_rad2.x, total_pos_rad2.y);


		ImVec2 end = total_pos_rad_vec22;

		win->DrawList->AddRectFilled(start, end, color, rounding);

		const int LINE_COUNT = 5;
		const ImVec2 text_offset(0, GetTextLineHeightWithSpacing());
		for (int i = 0; i < LINE_COUNT; ++i)
		{
			float2 tmp_pos(GetWindowContentRegionMin().x, GetWindowContentRegionMin().y);
			float2 tmp_pos3(win->Pos.x, win->Pos.y);
			float2 tmp_time_line_radius(TIMELINE_RADIUS, 0);
			float2 total_pos_rad = tmp_pos + tmp_pos3 + tmp_time_line_radius;
			ImVec2 total_pos_rad_vec2(total_pos_rad.x, total_pos_rad.y);

			ImVec2 a = total_pos_rad_vec2;
			a.x += i * GetWindowContentRegionWidth() / LINE_COUNT;
			ImVec2 b = a;
			b.y = start.y;
			win->DrawList->AddLine(a, b, line_color);
			char tmp[256];
			ImFormatString(tmp, sizeof(tmp), "%.2f", i * s_max_timeline_value / LINE_COUNT);
			win->DrawList->AddText(b, text_color, tmp);
		}

		EndChild();
	}


} // namespace ImGui