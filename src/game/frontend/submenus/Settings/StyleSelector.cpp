#include "StyleSelector.hpp"
#include "core/commands/ListCommand.hpp"
#include "core/frontend/manager/styles/StyleManager.hpp"

namespace YimMenu
{
	// Build style options statically
	static const std::vector<std::pair<int, const char*>> g_StyleOptions = [] {
		std::vector<std::pair<int, const char*>> options;
		int index = 0;
		for (const auto& [name, _] : StyleManager::GetStyles())
			options.emplace_back(index++, name.c_str());
		return options;
	}();

	// Custom command class
	class StyleSelector : public ListCommand
	{
	public:
		using ListCommand::ListCommand;

		void OnChange() override
		{
			const auto& styles = StyleManager::GetStyles();
			int selectedIndex = GetState();

			if (selectedIndex >= 0 && selectedIndex < static_cast<int>(styles.size()))
			{
				auto it = styles.begin();
				std::advance(it, selectedIndex);
				StyleManager::ApplyStyle(it->first);
			}
		}
	};

	// Create instance
	static StyleSelector g_InternalStyleSelector{
	    "styleselector",
	    "UI Style",
	    "Choose the UI style",
	    g_StyleOptions,
	    0};

	// Expose as reference
	ListCommand& g_StyleSelector = g_InternalStyleSelector;
}



