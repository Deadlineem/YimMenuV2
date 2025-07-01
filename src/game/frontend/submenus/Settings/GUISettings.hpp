#pragma once

#include "core/frontend/manager/Category.hpp"
#include "game/frontend/items/Items.hpp"
#include <memory>

namespace YimMenu
{
	std::shared_ptr<Category> DrawGUISettingsMenu();
	void InitializeColorCommands();
	void SyncColorCommandsToStyle();
	static void DrawStyleVec2(const char* label, float& x, float& y, float min, float max);
	static void DrawStyleFloat(const char* label, float& v, float min, float max);
}
