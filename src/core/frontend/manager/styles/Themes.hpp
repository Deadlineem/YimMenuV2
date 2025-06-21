#pragma once

#include <string>
#include <unordered_map>
#include <imgui.h>

namespace YimMenu
{
	// Forward declarations of each theme's render function, append when adding new themes
	extern void RenderClassicTheme();
	extern void RenderModernTheme();
	extern void RenderModernVTheme();
	extern void RenderModularTheme();

	// Theme settings save/load (Optional)
	// use these in your themes to save/load positions on movable/resizable items
	void SaveThemeSettings(const std::string& themeName,
	    const std::unordered_map<std::string, ImVec2>& buttonPositions,
	    const std::unordered_map<std::string, ImVec2>& buttonSizes,
	    const ImVec2& categoryPos,
	    const ImVec2& categorySize,
	    const ImVec2& contentPos,
	    const ImVec2& contentSize);

	bool LoadThemeSettings(const std::string& themeName,
	    std::unordered_map<std::string, ImVec2>& buttonPositions,
	    std::unordered_map<std::string, ImVec2>& buttonSizes,
	    ImVec2& categoryPos,
	    ImVec2& categorySize,
	    ImVec2& contentPos,
	    ImVec2& contentSize);
}
