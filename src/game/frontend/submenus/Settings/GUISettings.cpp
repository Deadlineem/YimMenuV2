#include "GUISettings.hpp"
#include "core/commands/ColorCommand.hpp"
#include "core/commands/Command.hpp"
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <Windows.h>

namespace YimMenu
{
	static std::vector<std::unique_ptr<ColorCommand>> g_ColorCommands;
	static bool g_ColorCommandsInitialized = false;
	static bool g_RoundingInitialized = false;

	static std::unordered_map<std::string, float> g_RoundingValues;

	static const std::string kSettingsFile = [] {
		char* appDataPath = nullptr;
		size_t len = 0;
		_dupenv_s(&appDataPath, &len, "APPDATA");
		std::string fullPath = std::string(appDataPath ? appDataPath : "") + "\\YimMenuV2\\GUISettings.json";
		free(appDataPath);
		return fullPath;
	}();

	void SyncColorCommandsToStyle();
	static void SyncRoundingToStyle();

	static void LoadColorSettings()
	{
		if (!std::filesystem::exists(kSettingsFile))
			return;

		std::ifstream file(kSettingsFile);
		if (!file.is_open())
			return;

		nlohmann::json json;
		file >> json;
		file.close();

		for (int i = 0; i < ImGuiCol_COUNT; ++i)
		{
			const char* name = ImGui::GetStyleColorName(i);
			auto it = json.find(name);
			if (it != json.end() && it->is_array() && it->size() == 4)
			{
				ImVec4 color = ImVec4((*it)[0], (*it)[1], (*it)[2], (*it)[3]);
				g_ColorCommands[i]->SetState(color);
			}
		}

		// Load rounding values
		const char* roundingKeys[] = {
		    "WindowRounding",
		    "FrameRounding",
		    "GrabRounding",
		    "ScrollbarRounding",
		    "ChildRounding",
		    "PopupRounding",
		    "TabRounding"};

		for (const char* key : roundingKeys)
		{
			auto it = json.find(key);
			if (it != json.end() && it->is_number())
				g_RoundingValues[key] = *it;
		}
	}

	static void SaveColorSettings()
	{
		nlohmann::json json;

		for (int i = 0; i < ImGuiCol_COUNT; ++i)
		{
			const char* name = ImGui::GetStyleColorName(i);
			const ImVec4& color = g_ColorCommands[i]->GetState();
			json[name] = {color.x, color.y, color.z, color.w};
		}

		for (const auto& [key, value] : g_RoundingValues)
		{
			json[key] = value;
		}

		std::filesystem::create_directories(std::filesystem::path(kSettingsFile).parent_path());

		std::ofstream file(kSettingsFile);
		if (file.is_open())
		{
			file << json.dump(4);
			file.close();
		}
	}

	void SyncColorCommandsToStyle()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		for (int i = 0; i < ImGuiCol_COUNT; i++)
			style.Colors[i] = g_ColorCommands[i]->GetState();
	}

	static void SyncRoundingToStyle()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = g_RoundingValues["WindowRounding"];
		style.FrameRounding = g_RoundingValues["FrameRounding"];
		style.GrabRounding = g_RoundingValues["GrabRounding"];
		style.ScrollbarRounding = g_RoundingValues["ScrollbarRounding"];
		style.ChildRounding = g_RoundingValues["ChildRounding"];
		style.PopupRounding = g_RoundingValues["PopupRounding"];
		style.TabRounding = g_RoundingValues["TabRounding"];
	}

	static void InitializeRoundingValues()
	{
		if (g_RoundingInitialized)
			return;

		ImGuiStyle& style = ImGui::GetStyle();
		g_RoundingValues["WindowRounding"] = style.WindowRounding;
		g_RoundingValues["FrameRounding"] = style.FrameRounding;
		g_RoundingValues["GrabRounding"] = style.GrabRounding;
		g_RoundingValues["ScrollbarRounding"] = style.ScrollbarRounding;
		g_RoundingValues["ChildRounding"] = style.ChildRounding;
		g_RoundingValues["PopupRounding"] = style.PopupRounding;
		g_RoundingValues["TabRounding"] = style.TabRounding;

		g_RoundingInitialized = true;
	}

	void InitializeColorCommands()
	{
		if (g_ColorCommandsInitialized)
			return;

		ImGuiStyle& style = ImGui::GetStyle();
		g_ColorCommands.reserve(ImGuiCol_COUNT);

		for (int i = 0; i < ImGuiCol_COUNT; i++)
		{
			const char* name = ImGui::GetStyleColorName(i);
			g_ColorCommands.emplace_back(std::make_unique<ColorCommand>(
			    "ColorCommand." + std::string(name),
			    name,
			    "Edit color for " + std::string(name),
			    style.Colors[i]));
		}

		InitializeRoundingValues();
		LoadColorSettings();
		SyncColorCommandsToStyle();
		SyncRoundingToStyle();
		g_ColorCommandsInitialized = true;
	}

	static void DrawColorsTab()
	{
		bool modified = false;

		ImGui::Text("Modify Colors:");
		ImGui::Separator();

		for (int i = 0; i < ImGuiCol_COUNT; i++)
		{
			const char* name = ImGui::GetStyleColorName(i);
			ImVec4 color = g_ColorCommands[i]->GetState();

			if (ImGui::ColorEdit4(name, (float*)&color))
			{
				g_ColorCommands[i]->SetState(color);
				modified = true;
			}
		}

		if (modified)
		{
			SyncColorCommandsToStyle();
			SaveColorSettings();
		}
	}

	static void DrawRoundingTab()
	{
		bool modified = false;

		ImGui::Text("Adjust Rounding:");
		ImGui::Separator();

		for (auto& [key, value] : g_RoundingValues)
		{
			if (ImGui::SliderFloat(key.c_str(), &value, 0.0f, 20.0f, "%.1f"))
				modified = true;
		}

		if (modified)
		{
			SyncRoundingToStyle();
			SaveColorSettings();
		}
	}

	std::shared_ptr<Category> DrawGUISettingsMenu()
	{
		InitializeColorCommands();

		ImGui::Text("ImGui Style Editor");
		ImGui::Separator();

		if (ImGui::BeginTabBar("StyleTabs"))
		{
			if (ImGui::BeginTabItem("Colors"))
			{
				DrawColorsTab();
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Rounding"))
			{
				DrawRoundingTab();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		auto ImGuiEditor = std::make_shared<Category>("UI Editor");
		ImGuiEditor->AddItem(std::make_unique<ImGuiItem>([] {
			DrawGUISettingsMenu();
		}));
		return ImGuiEditor;
	}
}
