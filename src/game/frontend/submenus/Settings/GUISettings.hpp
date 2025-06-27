#pragma once

#include "core/frontend/manager/Category.hpp"
#include "game/frontend/items/Items.hpp"
#include <memory>

namespace YimMenu
{
	std::shared_ptr<Category> DrawGUISettingsMenu();
	void InitializeColorCommands();
	void SyncColorCommandsToStyle();
}
