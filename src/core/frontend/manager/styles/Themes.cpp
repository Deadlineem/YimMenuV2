#include "Themes.hpp"
#include <nlohmann/json.hpp>
#include <Windows.h>
#include <fstream>
#include <filesystem>

using json = nlohmann::json;

namespace YimMenu
{
	static std::string GetSettingsFilePath(const std::string& themeName)
	{
		char* appData = nullptr;
		size_t len = 0;
		_dupenv_s(&appData, &len, "APPDATA");

		std::string basePath = std::string(appData) + "\\YimMenuV2\\Themes\\" + themeName;
		free(appData);

		std::filesystem::create_directories(basePath);

		return basePath + "\\" + themeName + ".json";
	}

	void SaveThemeSettings(const std::string& themeName,
	    const std::unordered_map<std::string, ImVec2>& buttonPositions,
	    const std::unordered_map<std::string, ImVec2>& buttonSizes,
	    const ImVec2& categoryPos,
	    const ImVec2& categorySize,
	    const ImVec2& contentPos,
	    const ImVec2& contentSize)
	{
		std::string filePath = GetSettingsFilePath(themeName);

		json j;

		// Buttons with positions and sizes
		for (const auto& [name, pos] : buttonPositions)
		{
			json btn;
			btn["pos"] = {{"x", pos.x}, {"y", pos.y}};

			if (buttonSizes.contains(name))
			{
				const ImVec2& size = buttonSizes.at(name);
				btn["size"] = {{"x", size.x}, {"y", size.y}};
			}
			j["Buttons"][name] = btn;
		}

		// Category Window
		j["CategoryWindow"]["pos"] = {{"x", categoryPos.x}, {"y", categoryPos.y}};
		j["CategoryWindow"]["size"] = {{"x", categorySize.x}, {"y", categorySize.y}};

		// Content Window
		j["ContentWindow"]["pos"] = {{"x", contentPos.x}, {"y", contentPos.y}};
		j["ContentWindow"]["size"] = {{"x", contentSize.x}, {"y", contentSize.y}};

		std::ofstream out(filePath);
		if (out.is_open())
		{
			out << j.dump(4);
			out.close();
		}
	}

	bool LoadThemeSettings(const std::string& themeName,
	    std::unordered_map<std::string, ImVec2>& buttonPositions,
	    std::unordered_map<std::string, ImVec2>& buttonSizes,
	    ImVec2& categoryPos,
	    ImVec2& categorySize,
	    ImVec2& contentPos,
	    ImVec2& contentSize)
	{
		std::string filePath = GetSettingsFilePath(themeName);

		std::ifstream in(filePath);
		if (!in.is_open())
			return false;

		json j;
		in >> j;
		in.close();

		try
		{
			if (j.contains("Buttons"))
			{
				for (const auto& [name, btn] : j["Buttons"].items())
				{
					if (btn.contains("pos"))
					{
						float x = btn["pos"].at("x").get<float>();
						float y = btn["pos"].at("y").get<float>();
						buttonPositions[name] = ImVec2(x, y);
					}
					if (btn.contains("size"))
					{
						float sx = btn["size"].at("x").get<float>();
						float sy = btn["size"].at("y").get<float>();
						buttonSizes[name] = ImVec2(sx, sy);
					}
				}
			}

			if (j.contains("CategoryWindow"))
			{
				auto& cat = j["CategoryWindow"];
				if (cat.contains("pos"))
				{
					categoryPos.x = cat["pos"].at("x").get<float>();
					categoryPos.y = cat["pos"].at("y").get<float>();
				}
				if (cat.contains("size"))
				{
					categorySize.x = cat["size"].at("x").get<float>();
					categorySize.y = cat["size"].at("y").get<float>();
				}
			}

			if (j.contains("ContentWindow"))
			{
				auto& content = j["ContentWindow"];
				if (content.contains("pos"))
				{
					contentPos.x = content["pos"].at("x").get<float>();
					contentPos.y = content["pos"].at("y").get<float>();
				}
				if (content.contains("size"))
				{
					contentSize.x = content["size"].at("x").get<float>();
					contentSize.y = content["size"].at("y").get<float>();
				}
			}

			return true;
		}
		catch (...)
		{
			return false;
		}
	}
}
