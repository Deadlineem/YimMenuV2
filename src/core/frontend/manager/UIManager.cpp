#include "UIManager.hpp"
#include "game/pointers/Pointers.hpp"
#include "game/frontend/Menu.hpp"
#include "core/frontend/manager/styles/StyleManager.hpp"
#include "game/frontend/submenus/Settings/StyleSelector.hpp"
#include "core/commands/ListCommand.hpp"

namespace YimMenu
{
	void UIManager::AddSubmenuImpl(const std::shared_ptr<Submenu>&& submenu)
	{
		if (!m_ActiveSubmenu)
			m_ActiveSubmenu = submenu;

		m_Submenus.push_back(std::move(submenu));
	}

	void UIManager::SetActiveSubmenuImpl(const std::shared_ptr<Submenu> Submenu)
	{
		m_ActiveSubmenu = Submenu;
	}

	void UIManager::DrawImpl()
	{
		const auto& styles = StyleManager::GetStyles();
		int selectedIndex = g_StyleSelector.GetState();
		g_StyleWasJustRestored = false;

		if (selectedIndex >= 0 && selectedIndex < static_cast<int>(styles.size()))
		{
			auto it = styles.begin();
			std::advance(it, selectedIndex);
			g_StyleWasJustRestored = true;
			YimMenu::StyleManager::ApplyStyle(it->first.c_str());
		}
	}

	std::shared_ptr<Submenu> UIManager::GetActiveSubmenuImpl()
	{
		if (m_ActiveSubmenu)
		{
			return m_ActiveSubmenu;
		}

		return nullptr;
	}

	std::shared_ptr<Category> UIManager::GetActiveCategoryImpl()
	{
		if (m_ActiveSubmenu)
		{
			return m_ActiveSubmenu->GetActiveCategory();
		}

		return nullptr;
	}
}