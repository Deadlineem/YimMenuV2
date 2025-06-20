#include "UIManager.hpp"
#include "game/pointers/Pointers.hpp"
#include "game/frontend/Menu.hpp"
#include "game/frontend/submenus/Settings/StyleSelector.hpp"
#include "core/commands/ListCommand.hpp"

extern void ApplyClassicTheme();
extern void ApplyModernTheme();
extern void ApplyModernVTheme();

namespace YimMenu
{
	void UIManager::AddSubmenuImpl(const std::shared_ptr<Submenu>&& submenu)
	{
		if (!m_ActiveSubmenu)
			m_ActiveSubmenu = submenu;

		m_Submenus.push_back(std::move(submenu));
	}

	void UIManager::SetActiveSubmenuImpl(const std::shared_ptr<Submenu> submenu)
	{
		m_ActiveSubmenu = submenu;
	}

	void UIManager::DrawImpl()
	{
		int selectedIndex = g_StyleSelector.GetState();

		switch (static_cast<UITheme>(selectedIndex))
		{
		case UITheme::Classic:
			ApplyClassicTheme();
			break;
		case UITheme::Modern:
			ApplyModernTheme();
			break;
		case UITheme::ModernV:
			ApplyModernVTheme();
			break;
		default:
			ApplyClassicTheme();
			break;
		}
	}

	std::shared_ptr<Submenu> UIManager::GetActiveSubmenuImpl()
	{
		return m_ActiveSubmenu;
	}

	std::shared_ptr<Category> UIManager::GetActiveCategoryImpl()
	{
		if (m_ActiveSubmenu)
			return m_ActiveSubmenu->GetActiveCategory();

		return nullptr;
	}
}