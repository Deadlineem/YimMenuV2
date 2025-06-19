#include "StyleManager.hpp"
#include <algorithm>

namespace YimMenu
{
	void StyleManager::RegisterStyle(const std::string& name, StyleFunc func)
	{
		s_Styles[name] = std::move(func);
	}

	const std::map<std::string, StyleManager::StyleFunc>& StyleManager::GetStyles()
	{
		return s_Styles;
	}

	void StyleManager::ApplyStyle(const std::string& name)
	{
		auto it = s_Styles.find(name);
		if (it != s_Styles.end())
		{
			s_CurrentStyle = name;
			it->second();
		}
	}
}