#include <algorithm> // For std::clamp
#include "game/pointers/Pointers.hpp"
#include "game/frontend/Menu.hpp"
#include "core/frontend/manager/UIManager.hpp"

namespace YimMenu
{
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

		for (size_t i = 0; i < submenus.size(); ++i)
		{
			auto& submenu = submenus[i];
			std::string windowId = "NavWin_" + submenu->m_Name;

			ImVec2 startPos(leftOffset, topOffset + i * (defaultHeight + bubblePadding));
			ImVec2 defaultSize(defaultWidth, defaultHeight);

			ImGui::SetNextWindowPos(startPos, ImGuiCond_Once);
			ImGui::SetNextWindowSize(defaultSize, ImGuiCond_Once);
			ImGui::SetNextWindowSizeConstraints(ImVec2(60, 80), ImVec2(200, 200));
			ImGui::SetNextWindowBgAlpha(0.0f); // Invisible background for drag + draw overlay

			if (ImGui::Begin(windowId.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
			{
				ImVec2 winPos = ImGui::GetWindowPos();
				ImVec2 winSize = ImGui::GetWindowSize();
				ImVec2 center(winPos.x + winSize.x / 2, winPos.y + winSize.y / 2 - 10);

				ImU32 bgColor = IM_COL32(25, 25, 30, 240);
				ImU32 borderColor = IM_COL32(80, 80, 90, 160);
				ImU32 iconColor = IM_COL32(255, 255, 255, 255);
				ImU32 activeColor = IM_COL32(46, 204, 113, 255);

				// Bubble size inside window
				ImVec2 bubbleSize(winSize.x - 20, winSize.y - 30);
				ImVec2 bubblePos(winPos.x + (winSize.x - bubbleSize.x) / 2, winPos.y + 10);

				// Click logic via invisible button
				ImGui::SetCursorScreenPos(bubblePos);
				ImGui::InvisibleButton(("##bubble_" + submenu->m_Name).c_str(), bubbleSize);
				bool hovered = ImGui::IsItemHovered();
				bool clicked = ImGui::IsItemClicked();

				if (clicked)
				{
					if (activeSubmenu == submenu)
						UIManager::SetShowContentWindow(!UIManager::ShowingContentWindow());
					else
					{
						UIManager::SetActiveSubmenu(submenu);
						UIManager::SetShowContentWindow(true);
					}
				}

				// Draw bubble background
				drawList->AddRectFilled(bubblePos, bubblePos + bubbleSize, hovered ? IM_COL32(45, 45, 55, 255) : bgColor, 10.0f);
				drawList->AddRect(bubblePos, bubblePos + bubbleSize, borderColor, 10.0f, 0, 1.5f);

				// Icon (scaled based on size)
				ImGui::PushFont(Menu::Font::g_AwesomeFont);
				float iconScale = std::clamp(bubbleSize.y * 0.4f, 14.0f, 32.0f);
				ImVec2 iconSize = ImGui::CalcTextSize(submenu->m_Icon.c_str());
				ImVec2 iconPos(center.x - iconSize.x / 2, bubblePos.y + bubbleSize.y / 2 - iconSize.y / 2);
				drawList->AddText(Menu::Font::g_AwesomeFont, iconScale, iconPos, (submenu == activeSubmenu) ? activeColor : iconColor, submenu->m_Icon.c_str());
				ImGui::PopFont();

				// Label below bubble
				ImVec2 labelSize = ImGui::CalcTextSize(submenu->m_Name.c_str());
				ImVec2 labelPos(center.x - labelSize.x / 2, bubblePos.y + bubbleSize.y + 6);
				drawList->AddText(labelPos, iconColor, submenu->m_Name.c_str());
			}
			ImGui::End();
		}

		// CATEGORY SELECTOR WINDOW
		if (activeSubmenu)
		{
			ImGui::SetNextWindowSizeConstraints(ImVec2(100, 40), ImVec2(io.DisplaySize.x * 0.9f, 200));
			ImGui::SetNextWindowBgAlpha(0.92f);
			if (ImGui::Begin("##CategorySelectorWindow", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
			{
				activeSubmenu->DrawCategorySelectors();
			}
			ImGui::End();
		}

		// OPTIONS CONTENT WINDOW
		if (UIManager::ShowingContentWindow() && activeSubmenu)
		{
			ImVec2 defaultSize(*Pointers.ScreenResX / 2.8f, *Pointers.ScreenResY / 2.5f);
			ImGui::SetNextWindowSize(defaultSize, ImGuiCond_Once);
			ImGui::SetNextWindowBgAlpha(0.94f);

			if (ImGui::Begin("##OptionsContentWindow", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
			{
				ImFont* optionsFont = UIManager::GetOptionsFont();
				if (optionsFont)
					ImGui::PushFont(optionsFont);
				activeSubmenu->Draw();
				if (optionsFont)
					ImGui::PopFont();
			}
			ImGui::End();
		}
	}
}
