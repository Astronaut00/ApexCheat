#pragma once
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <d3d11.h>

#undef DrawText
class Render
{
public:
	static inline void Text(float x, float y, const char* text, bool outlined, ImColor color...)
	{
		if (outlined)
		{
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(x, y + 1.0f), ImColor(0, 0, 0, 255), text);
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(x + 1.0f, y), ImColor(0, 0, 0, 255), text);
		}

		ImGui::GetBackgroundDrawList()->AddText(ImVec2(x, y), color, text);;
	}

	static inline void Text(ImFont* font, float size, float x, float y, const char* text, bool outlined, ImColor color...)
	{
		if (outlined)
		{
			ImGui::GetBackgroundDrawList()->AddText(font, size, ImVec2(x, y + 1.0f), ImColor(0, 0, 0, 255), text);
			ImGui::GetBackgroundDrawList()->AddText(font, size, ImVec2(x + 1.0f, y), ImColor(0, 0, 0, 255), text);
		}

		ImGui::GetBackgroundDrawList()->AddText(font, size, ImVec2(x, y), color, text);;
	}

	static inline void Line(float x1, float y1, float x2, float y2, ImColor color, float thickness)
	{
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), color, thickness);
	}

	static inline void Rect(float x, float y, float w, float h, ImColor color, float rounding, float thickness)
	{
		ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, rounding, ImDrawFlags_None, thickness);
	}

	static inline void RectFilled(float x, float y, float w, float h, ImColor color, float rounding, float thickness)
	{
		ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, rounding, ImDrawFlags_None);
	}

	static inline void Triangle(float x1, float y1, float x2, float y2, float x3, float y3, ImColor color, float thickness)
	{
		ImGui::GetBackgroundDrawList()->AddTriangle(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), color, thickness);
	}

	static inline void TriangleFilled(float x1, float y1, float x2, float y2, float x3, float y3, ImColor color, float thickness)
	{
		ImGui::GetBackgroundDrawList()->AddTriangleFilled(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), color);
	}

	static inline void Circle(float x, float y, float radius, ImColor color, float thickness)
	{
		ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(x, y), radius, color, ImDrawFlags_None, thickness);
	}

	static inline void CircleFilled(float x, float y, float radius, ImColor color, float thickness)
	{
		ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(x, y), radius, color, ImDrawFlags_None);
	}
};
