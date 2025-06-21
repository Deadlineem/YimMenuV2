#pragma once
#include <map>
#include <string>
#include <fstream>
#include <cstdlib>
#include <filesystem>
#include <imgui.h>
#include <nlohmann/json.hpp>

namespace YimMenu
{
	// Forward declarations of each theme's render function, append when adding new themes
	extern void RenderClassicTheme();
	extern void RenderModernTheme();
	extern void RenderModernVTheme();
	extern void RenderModularTheme();


	struct WindowMetrics
	{
		ImVec2 position;
		ImVec2 size;
	};

	using WindowMap = std::map<std::string, WindowMetrics>;

	inline void CreateThemeSettings(const std::string& themeName, const WindowMap& windows)
	{
		namespace fs = std::filesystem;
		fs::path themeDir = fs::path(std::getenv("appdata")) / "YimMenuV2" / "Themes" / themeName;
		fs::path themeFile = themeDir / (themeName + ".json");

		if (!fs::exists(themeDir))
			fs::create_directories(themeDir);

		nlohmann::json j;
		for (const auto& [id, wm] : windows)
		{
			j["windows"][id]["position"] = {wm.position.x, wm.position.y};
			j["windows"][id]["size"] = {wm.size.x, wm.size.y};
		}

		std::ofstream out(themeFile);
		if (out.is_open())
			out << j.dump(4);
	}

	inline WindowMap LoadThemeSettings(const std::string& themeName)
	{
		namespace fs = std::filesystem;
		fs::path themeFile = fs::path(std::getenv("appdata")) / "YimMenuV2" / "Themes" / themeName / (themeName + ".json");

		WindowMap result;
		std::ifstream in(themeFile);
		if (!in.is_open())
			return result;

		nlohmann::json j;
		in >> j;

		if (!j.contains("windows"))
			return result;

		for (const auto& [id, val] : j["windows"].items())
		{
			ImVec2 pos(0, 0), size(0, 0);
			if (val.contains("position") && val["position"].is_array() && val["position"].size() == 2)
			{
				pos = ImVec2(val["position"][0].get<float>(), val["position"][1].get<float>());
			}
			if (val.contains("size") && val["size"].is_array() && val["size"].size() == 2)
			{
				size = ImVec2(val["size"][0].get<float>(), val["size"][1].get<float>());
			}
			result[id] = {pos, size};
		}

		return result;
	}
}