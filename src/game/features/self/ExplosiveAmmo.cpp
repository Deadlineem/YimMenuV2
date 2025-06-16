#include "core/commands/LoopedCommand.hpp"
#include "game/backend/Self.hpp"
#include "game/gta/Pools.hpp"
#include "types/fx/ExplosionType.hpp"
#include <game/gta/Natives.hpp>
#include "core/commands/ListCommand.hpp"
#include "core/util/magic_enum.hpp"

namespace YimMenu::Features
{
	static std::vector<std::pair<int, const char*>> GetExplosionTypes()
	{
		std::vector<std::pair<int, const char*>> result;

		for (auto e : magic_enum::enum_values<ExplosionType>())
		{
			auto name = magic_enum::enum_name(e);
			result.emplace_back(static_cast<int>(e), name.data());
		}

		return result;
	}

	static ListCommand _SelectedExplosion{"selectedexplosion", "Explosion Type", "Select an explosion type", GetExplosionTypes(), static_cast<int>(ExplosionType::BULLET)};

	class ExplosiveAmmo : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnEnable() override
		{
			TriggerExplosion();
		}

		virtual void OnTick() override
		{
			TriggerExplosion();
		}

		void TriggerExplosion()
		{
			Vector3 aimCoords;
			if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(Self::GetPed().GetHandle(), &aimCoords))
			{
				int selected = _SelectedExplosion.GetState();
				auto explosionType = static_cast<ExplosionType>(selected);

				FIRE::ADD_OWNED_EXPLOSION(
				    Self::GetPed().GetHandle(),
				    aimCoords.x,
				    aimCoords.y,
				    aimCoords.z,
				    static_cast<int>(explosionType), // explosionType
				    100.0f,                          // damageScale
				    true,                            // isAudible
				    false,                           // isInvisible
				    0.1f                             // cameraShake
				);
			}
		}
	};

	static ExplosiveAmmo _ExplosiveAmmo{"ExplosiveAmmo", "Explosive Ammo", "Every bullet you shoot explodes."};
}
