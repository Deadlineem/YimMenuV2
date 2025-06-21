#include <algorithm>
#include <unordered_map>
#include "game/pointers/Pointers.hpp"
#include "game/frontend/Menu.hpp"
#include "core/frontend/manager/UIManager.hpp"
#include "core/frontend/manager/styles/Themes.hpp"

namespace YimMenu
{
	static const std::string themeName = "Modular";

	void RenderModularTheme()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImDrawList* drawList = ImGui::GetBackgroundDrawList();
		const auto& submenus = UIManager::GetSubmenus();
		auto activeSubmenu = UIManager::GetActiveSubmenu();

		const float bubblePadding = 10.0f;
		const float leftOffset = 25.0f;
		const float topOffset = 25.0f;
		const float defaultWidth = 80.0f;
		const float defaultHeight = 100.0f;

		static bool initialized = false;

		// Separate data containers
		static std::unordered_map<std::string, ImVec2> buttonPositions;
		static std::unordered_map<std::string, ImVec2> buttonSizes;
		static ImVec2 categoryPos(300, 100);
		static ImVec2 categorySize(300, 100); // placeholder size, overridden by constraints
		static ImVec2 contentPos(100, 300);
		static ImVec2 contentSize(*Pointers.ScreenResX / 2.8f, *Pointers.ScreenResY / 2.5f);

		if (!initialized)
		{
			LoadThemeSettings(themeName, buttonPositions, buttonSizes, categoryPos, categorySize, contentPos, contentSize);
			initialized = true;
		}

		bool anyWindowMovedOrResized = false;

		// SUBMENU BUBBLES
		for (size_t i = 0; i < submenus.size(); ++i)
		{
			auto& submenu = submenus[i];
			std::string windowId = "NavWin_" + submenu->m_Name;

			ImVec2 defaultPos(leftOffset, topOffset + i * (defaultHeight + bubblePadding));
			ImVec2 pos = buttonPositions.contains(submenu->m_Name) ? buttonPositions[submenu->m_Name] : defaultPos;
			ImVec2 size = buttonSizes.contains(submenu->m_Name) ? buttonSizes[submenu->m_Name] : ImVec2(defaultWidth, defaultHeight);

			ImGui::SetNextWindowPos(pos, ImGuiCond_Once);
			ImGui::SetNextWindowSize(size, ImGuiCond_Once);
			ImGui::SetNextWindowSizeConstraints(ImVec2(60, 80), ImVec2(200, 200));
			ImGui::SetNextWindowBgAlpha(0.0f);

			if (ImGui::Begin(windowId.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
			{
				ImVec2 currentPos = ImGui::GetWindowPos();
				ImVec2 currentSize = ImGui::GetWindowSize();

				if (currentPos.x != pos.x || currentPos.y != pos.y)
				{
					buttonPositions[submenu->m_Name] = currentPos;
					anyWindowMovedOrResized = true;
				}
				if (currentSize.x != size.x || currentSize.y != size.y)
				{
					buttonSizes[submenu->m_Name] = currentSize;
					anyWindowMovedOrResized = true;
				}

				ImVec2 center(currentPos.x + currentSize.x / 2, currentPos.y + currentSize.y / 2 - 10);

				ImU32 bgColor = IM_COL32(25, 25, 30, 240);
				ImU32 borderColor = IM_COL32(80, 80, 90, 160);
				ImU32 iconColor = IM_COL32(255, 255, 255, 255);
				ImU32 activeColor = IM_COL32(46, 204, 113, 255);

				ImVec2 bubbleSize(currentSize.x - 20, currentSize.y - 30);
				ImVec2 bubblePos(currentPos.x + (currentSize.x - bubbleSize.x) / 2, currentPos.y + 10);

				ImGui::SetCursorScreenPos(bubblePos);
				ImGui::InvisibleButton(("##bubble_" + submenu->m_Name).c_str(), bubbleSize);

				if (ImGui::IsItemClicked())
				{
					if (activeSubmenu == submenu)
						UIManager::SetShowContentWindow(!UIManager::ShowingContentWindow());
					else
					{
						UIManager::SetActiveSubmenu(submenu);
						UIManager::SetShowContentWindow(true);
					}
				}

				drawList->AddRectFilled(bubblePos, bubblePos + bubbleSize, ImGui::IsItemHovered() ? IM_COL32(45, 45, 55, 255) : bgColor, 10.0f);
				drawList->AddRect(bubblePos, bubblePos + bubbleSize, borderColor, 10.0f, 0, 1.5f);

				ImGui::PushFont(Menu::Font::g_AwesomeFont);
				float iconScale = std::clamp(bubbleSize.y * 0.4f, 14.0f, 32.0f);
				ImVec2 iconSize = ImGui::CalcTextSize(submenu->m_Icon.c_str());
				ImVec2 iconPos(center.x - iconSize.x / 2, bubblePos.y + bubbleSize.y / 2 - iconSize.y / 2);
				drawList->AddText(Menu::Font::g_AwesomeFont, iconScale, iconPos, (submenu == activeSubmenu) ? activeColor : iconColor, submenu->m_Icon.c_str());
				ImGui::PopFont();

				ImVec2 labelSize = ImGui::CalcTextSize(submenu->m_Name.c_str());
				ImVec2 labelPos(center.x - labelSize.x / 2, bubblePos.y + bubbleSize.y + 6);
				drawList->AddText(labelPos, iconColor, submenu->m_Name.c_str());
			}
			ImGui::End();
		}

		// CATEGORY SELECTOR WINDOW
		if (activeSubmenu)
		{
			ImGui::SetNextWindowPos(categoryPos, ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSizeConstraints(ImVec2(100, 40), ImVec2(io.DisplaySize.x * 0.9f, 200));
			ImGui::SetNextWindowBgAlpha(0.92f);

			if (ImGui::Begin("##CategorySelectorWindow", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImVec2 newPos = ImGui::GetWindowPos();
				if (newPos.x != categoryPos.x || newPos.y != categoryPos.y)
				{
					categoryPos = newPos;
					anyWindowMovedOrResized = true;
				}

				// Always auto-resize window, so size isn't saved here. But if you want size saved, you can capture it similarly.

				activeSubmenu->DrawCategorySelectors();
			}
			ImGui::End();
		}

		// OPTIONS CONTENT WINDOW
		if (UIManager::ShowingContentWindow() && activeSubmenu)
		{
			ImGui::SetNextWindowPos(contentPos, ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(contentSize, ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowBgAlpha(0.94f);

			if (ImGui::Begin("##OptionsContentWindow", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
			{
				ImVec2 newPos = ImGui::GetWindowPos();
				ImVec2 newSize = ImGui::GetWindowSize();

				if (newPos.x != contentPos.x || newPos.y != contentPos.y)
				{
					contentPos = newPos;
					anyWindowMovedOrResized = true;
				}

				if (newSize.x != contentSize.x || newSize.y != contentSize.y)
				{
					contentSize = newSize;
					anyWindowMovedOrResized = true;
				}

				if (auto* font = UIManager::GetOptionsFont())
					ImGui::PushFont(font);
				activeSubmenu->Draw();
				if (auto* font = UIManager::GetOptionsFont())
					ImGui::PopFont();
			}
			ImGui::End();
		}

		if (anyWindowMovedOrResized)
		{
			SaveThemeSettings(themeName, buttonPositions, buttonSizes, categoryPos, categorySize, contentPos, contentSize);
		}
	}
}
