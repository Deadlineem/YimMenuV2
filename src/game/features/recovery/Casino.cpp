#include "core/commands/LoopedCommand.hpp"
#include "core/commands/Command.hpp"
#include "game/backend/Self.hpp"
#include "game/gta/Natives.hpp"
#include "game/gta/ScriptLocal.hpp"
#include "game/gta/Stats.hpp"
#include "game/features/recovery/Casino.hpp"

#include <set>


namespace YimMenu::Features
{
	class BypassCasinoBans : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;
		virtual void OnTick() override
		{
			// Bypass Casino Bans
			Stats::SetInt("MPPLY_CASINO_CHIPS_WON_GD", 0);
			Stats::SetInt("MPPLY_CASINO_CHIPS_WONTIM", 0);
			Stats::SetInt("MPPLY_CASINO_GMBLNG_GD", 0);
			Stats::SetInt("MPPLY_CASINO_BAN_TIME", 0);
			Stats::SetInt("MPPLY_CASINO_CHIPS_PURTIM", 0);
			Stats::SetInt("MPPLY_CASINO_CHIPS_PUR_GD", 0);
			Stats::SetInt("MPPLY_CASINO_CHIPS_SOLD", 0);
			Stats::SetInt("MPPLY_CASINO_CHIPS_SELTIM", 0);
		}
		virtual void OnDisable() override
		{
			// This may need some values, but will work as is for now.
		}
	};
	class CasinoManipulateRigSlotMachines : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		int slots_random_results_table = 1348;
		std::set<int> slots_blacklist = {9, 21, 22, 87, 152};

		virtual void OnTick() override
		{
			if (Scripts::SafeToModifyFreemodeBroadcastGlobals() && SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH("casino_slots"_J))
			{
				Player casinoSlotsScriptHostPlayer = NETWORK::NETWORK_GET_HOST_OF_SCRIPT("casino_slots", -1, 0);
				auto casinoSlotsScriptHostPlayerId = casinoSlotsScriptHostPlayer.GetId();
				auto selfPlayerId = Self::GetPlayer().GetId();
				if (casinoSlotsScriptHostPlayerId != selfPlayerId)
				{
					Scripts::ForceScriptHost(Scripts::FindScriptThread("casino_slots"_J));
				}

				bool needs_run = false;
				for (int slots_iter = 3; slots_iter <= 196; ++slots_iter)
				{
					if (!slots_blacklist.contains(slots_iter))
					{
						if (*ScriptLocal("casino_slots"_J, slots_random_results_table + slots_iter).As<int*>() != 6)
						{
							needs_run = true;
						}
					}
				}
				if (needs_run)
				{
					for (int slots_iter = 3; slots_iter <= 196; ++slots_iter)
					{
						if (!slots_blacklist.contains(slots_iter))
						{
							int slot_result = 6;
							*ScriptLocal("casino_slots"_J, slots_random_results_table + slots_iter).As<int*>() = slot_result;
						}
					}
				}
			}
		}

		virtual void OnDisable() override
		{
			if (Scripts::SafeToModifyFreemodeBroadcastGlobals() && SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH("casino_slots"_J))
			{
				Player casinoSlotsScriptHostPlayer = NETWORK::NETWORK_GET_HOST_OF_SCRIPT("casino_slots", -1, 0);
				auto casinoSlotsScriptHostPlayerId = casinoSlotsScriptHostPlayer.GetId();
				auto selfPlayerId = Self::GetPlayer().GetId();
				if (casinoSlotsScriptHostPlayerId != selfPlayerId)
				{
					Scripts::ForceScriptHost(Scripts::FindScriptThread("casino_slots"_J));
				}

				for (int slots_iter = 3; slots_iter <= 196; ++slots_iter)
				{
					if (!slots_blacklist.contains(slots_iter))
					{
						int slot_result = 6;
						std::srand(static_cast<unsigned int>(std::time(0)) + slots_iter);
						slot_result = std::rand() % 7; // Generates a pseudo random number between 0 and 7
						*ScriptLocal("casino_slots"_J, slots_random_results_table + slots_iter).As<int*>() = slot_result;
					}
				}
			}
		}
	};

	constexpr int prize_wheel_win_state = 280;
	constexpr int prize_wheel_prize = 14;
	constexpr int prize_wheel_prize_state = 45;

	constexpr int prize_offset = prize_wheel_win_state + prize_wheel_prize;
	constexpr int state_offset = prize_wheel_win_state + prize_wheel_prize_state;

	class SetLuckyWheelPrizeCommand : public Command
	{
	public:
		SetLuckyWheelPrizeCommand(const std::string& name, const std::string& label, const std::string& description, int prizeId) :
		    Command(name, label, description, 0),
		    m_PrizeId(prizeId)
		{
		}

		void OnCall() override
		{
			if (Scripts::SafeToModifyFreemodeBroadcastGlobals() && SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH("casino_lucky_wheel"_J))
			{
				Player host = NETWORK::NETWORK_GET_HOST_OF_SCRIPT("casino_lucky_wheel", -1, 0);
				if (host.GetId() != Self::GetPlayer().GetId())
				{
					Scripts::ForceScriptHost(Scripts::FindScriptThread("casino_lucky_wheel"_J));
				}

				*ScriptLocal("casino_lucky_wheel"_J, prize_offset).As<int*>() = m_PrizeId;
				*ScriptLocal("casino_lucky_wheel"_J, state_offset).As<int*>() = 11;
			}
		}

	private:
		int m_PrizeId;
	};

	static SetLuckyWheelPrizeCommand _PodiumVehicle("wheelpodium", "Give Podium Vehicle", "Sets prize to Podium Vehicle", 18);
	static SetLuckyWheelPrizeCommand _MysteryPrize("wheelmystery", "Give Mystery Prize", "Sets prize to Mystery", 11);
	static SetLuckyWheelPrizeCommand _CashPrize("wheelcash", "Give $50,000", "Sets prize to $50,000", 19);
	static SetLuckyWheelPrizeCommand _ChipsPrize("wheelchips", "Give 25,000 Chips", "Sets prize to Chips", 15);
	static SetLuckyWheelPrizeCommand _RpPrize("wheelrp", "Give 15,000 RP", "Sets prize to RP", 17);
	static SetLuckyWheelPrizeCommand _DiscountPrize("wheeldiscount", "Give Discount", "Sets prize to Discount", 4);
	static SetLuckyWheelPrizeCommand _ClothingPrize("wheelclothing", "Give Clothing", "Sets prize to Clothing", 8);


	static BypassCasinoBans _BypassCasinoBans{"bypasscasinobans", "Bypass Casino Ban", "Bypasses the Casino Ban and cooldown allowing you to manipulate the machines/tables as much as you want"};
	static CasinoManipulateRigSlotMachines _CasinoManipulateRigSlotMachines{"casinomanipulaterigslotmachines", "Manipulate Rig Slot Machines", "Lets you win the Rig Slot Machines every time"};

}