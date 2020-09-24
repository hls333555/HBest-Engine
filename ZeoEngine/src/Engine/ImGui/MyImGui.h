#pragma once

#include <imgui.h>

namespace ImGui {

	IMGUI_API bool DragInt_8(const char* label, int8_t* v, float v_speed = 1.0f, int8_t v_min = 0i8, int8_t v_max = 0i8, const char* format = "%d");
	IMGUI_API bool DragInt_32(const char* label, int32_t* v, float v_speed = 1.0f, int32_t v_min = 0i32, int32_t v_max = 0i32, const char* format = "%d");
	IMGUI_API bool DragInt_64(const char* label, int64_t* v, float v_speed = 1.0f, int64_t v_min = 0i64, int64_t v_max = 0i64, const char* format = "%d");
	IMGUI_API bool DragUInt_8(const char* label, uint8_t* v, float v_speed = 1.0f, uint8_t v_min = 0ui8, uint8_t v_max = 0ui8, const char* format = "%d");
	IMGUI_API bool DragUInt_32(const char* label, uint32_t* v, float v_speed = 1.0f, uint32_t v_min = 0ui32, uint32_t v_max = 0ui32, const char* format = "%d");
	IMGUI_API bool DragUInt_64(const char* label, uint64_t* v, float v_speed = 1.0f, uint64_t v_min = 0ui64, uint64_t v_max = 0ui64, const char* format = "%d");
	IMGUI_API bool DragDouble(const char* label, double* v, float v_speed = 1.0f, double v_min = 0.0, double v_max = 0.0, const char* format = "%.3f", float power = 1.0f);

	IMGUI_API void TextCentered(const char* fmt, ...);

	/** To draw a progress bar in Game View window, use GetWindowDrawList(); to draw a progress bar in standalone game, use GetForgroundDrawList(). */
	IMGUI_API void AddProgressBar(ImDrawList* drawList, float fraction, const ImVec2& a, const ImVec2& b, ImU32 foregroundCol, ImU32 backgroundCol);
}

namespace ZeoEngine {
	
	struct ImVec2Data
	{
		ImVec2 Data;
		ImGuiCond Condition{ ImGuiCond_FirstUseEver };

		bool operator==(const ImVec2Data& other) const
		{
			return Data.x == other.Data.x && Data.y == other.Data.y && Condition == other.Condition;
		}

		static ImVec2Data DefaultPos; // Center of main viewport
		static ImVec2Data DefaultSize;
	};

}
