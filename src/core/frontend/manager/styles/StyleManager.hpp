#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>

namespace YimMenu
{
	class StyleManager
	{
	public:
		using StyleFunc = std::function<void()>;

		static void RegisterStyle(const std::string& name, StyleFunc func);
		static const std::map<std::string, StyleFunc>& GetStyles();
		static void ApplyStyle(const std::string& name);

	private:
		inline static std::map<std::string, StyleFunc> s_Styles;
		inline static std::string s_CurrentStyle;
	};
}