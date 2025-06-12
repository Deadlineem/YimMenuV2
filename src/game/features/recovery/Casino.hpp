#pragma once
#include "core/commands/Command.hpp"

namespace YimMenu::Features
{
	class SetLuckyWheelPrizeCommand : public Command
	{
	public:
		SetLuckyWheelPrizeCommand(const std::string& name, const std::string& label, const std::string& description, int prizeValue);

	protected:
		void OnCall() override;

	private:
		int m_PrizeValue;
	};

	// Declare the instances (they'll be defined in .cpp)
	extern SetLuckyWheelPrizeCommand _PodiumVehicle;
	extern SetLuckyWheelPrizeCommand _MysteryPrize;
	extern SetLuckyWheelPrizeCommand _CashPrize;
	extern SetLuckyWheelPrizeCommand _ChipsPrize;
	extern SetLuckyWheelPrizeCommand _RpPrize;
	extern SetLuckyWheelPrizeCommand _DiscountPrize;
	extern SetLuckyWheelPrizeCommand _ClothingPrize;
}
