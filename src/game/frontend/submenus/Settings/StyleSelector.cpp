#include "StyleSelector.hpp"
#include "core/commands/ListCommand.hpp"
#include "core/frontend/manager/UIManager.hpp"

namespace YimMenu
{
	static const std::vector<std::pair<int, const char*>> g_StyleOptions = {
	    {0, "Classic"},
	    {1, "Modern"},
	    {2, "ModernV"},
	    {3, "Modular"},
	};

	static ListCommand g_InternalStyleSelector{
	    "styleselector",
	    "UI Style",
	    "Choose the UI style",
	    g_StyleOptions,
	    0};

	ListCommand& g_StyleSelector = g_InternalStyleSelector;
}
