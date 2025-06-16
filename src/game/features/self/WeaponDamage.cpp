#include "core/commands/LoopedCommand.hpp"
#include "game/backend/Self.hpp"
#include "game/gta/Pools.hpp"
#include "types/fx/ExplosionType.hpp"
#include <game/gta/Natives.hpp>
#include "core/commands/FloatCommand.hpp"

namespace YimMenu::Features
{
	static FloatCommand _WeaponDamageScale{"weapondamagescale", "Weapon Damage Scale", "Sets the damage scale for weapons (including melee)", 0.0f, 10.0f, 1.0f};

	class WeaponDamage : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		void OnTick() override
		{
			float scale = _WeaponDamageScale.GetState();
			if (scale > 0.0f)
			{
				Hash weapon;
				if (WEAPON::GET_CURRENT_PED_WEAPON(Self::GetPed().GetHandle(), &weapon, 0))
				{
					WEAPON::SET_WEAPON_DAMAGE_MODIFIER(weapon, scale);
					WEAPON::_SET_WEAPON_PED_DAMAGE_MODIFIER(Self::GetPed().GetHandle(), scale);
					PLAYER::SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER(Self::GetPlayer().GetId(), scale, 1);

					Hash target = PED::GET_MELEE_TARGET_FOR_PED(Self::GetPed().GetHandle());
					if (target != 0)
					{
						Vector3 forward = ENTITY::GET_ENTITY_FORWARD_VECTOR(Self::GetPed().GetHandle());

						if (ENTITY::IS_ENTITY_A_PED(target))
						{
							float forceScale = scale * 1.0f; // Set ped force lower since they weigh less
						}
						float forceScale = scale * 5.0f;
						Vector3 force{
						    forward.x * forceScale,
						    forward.y * forceScale,
						    0.1f // Small vertical lift
						};

						ENTITY::APPLY_FORCE_TO_ENTITY(
						    target,
						    0,
						    force.x,
						    force.y,
						    force.z,
						    0.0f,
						    0.0f,
						    0.0f,
						    true,
						    true,
						    true,
						    true,
						    false,
						    true);
					}
				}
			}
		}
	};

	static WeaponDamage _WeaponDamage{"weapondamage", "Weapon Damage", "Allows altering your weapons damage output (includes melee)"};
}