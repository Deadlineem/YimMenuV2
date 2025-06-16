#include "core/commands/LoopedCommand.hpp"
#include "game/backend/Self.hpp"
#include "game/gta/Pools.hpp"
#include "types/fx/ExplosionType.hpp"
#include <game/gta/Natives.hpp>
#include "core/commands/ListCommand.hpp"

namespace YimMenu::Features
{
	static std::vector<std::pair<int, const char*>> explosionTypes = {
	    {static_cast<int>(ExplosionType::DONTCARE), "Don't Care"},
	    {static_cast<int>(ExplosionType::GRENADE), "Grenade"},
	    {static_cast<int>(ExplosionType::GRENADELAUNCHER), "Grenade Launcher"},
	    {static_cast<int>(ExplosionType::STICKYBOMB), "Sticky Bomb"},
	    {static_cast<int>(ExplosionType::MOLOTOV), "Molotov"},
	    {static_cast<int>(ExplosionType::ROCKET), "Rocket"},
	    {static_cast<int>(ExplosionType::TANKSHELL), "Tank Shell"},
	    {static_cast<int>(ExplosionType::HI_OCTANE), "Hi-Octane"},
	    {static_cast<int>(ExplosionType::CAR), "Car"},
	    {static_cast<int>(ExplosionType::PLANE), "Plane"},
	    {static_cast<int>(ExplosionType::PETROL_PUMP), "Petrol Pump"},
	    {static_cast<int>(ExplosionType::BIKE), "Bike"},
	    {static_cast<int>(ExplosionType::DIR_STEAM), "Steam"},
	    {static_cast<int>(ExplosionType::DIR_FLAME), "Flame"},
	    {static_cast<int>(ExplosionType::DIR_WATER_HYDRANT), "Water Hydrant"},
	    {static_cast<int>(ExplosionType::DIR_GAS_CANISTER), "Gas Canister"},
	    {static_cast<int>(ExplosionType::BOAT), "Boat"},
	    {static_cast<int>(ExplosionType::SHIP_DESTROY), "Ship Destroy"},
	    {static_cast<int>(ExplosionType::TRUCK), "Truck"},
	    {static_cast<int>(ExplosionType::BULLET), "Bullet"},
	    {static_cast<int>(ExplosionType::SMOKEGRENADELAUNCHER), "Smoke Grenade Launcher"},
	    {static_cast<int>(ExplosionType::SMOKEGRENADE), "Smoke Grenade"},
	    {static_cast<int>(ExplosionType::BZGAS), "BZ Gas"},
	    {static_cast<int>(ExplosionType::FLARE), "Flare"},
	    {static_cast<int>(ExplosionType::GAS_CANISTER), "Gas Canister"},
	    {static_cast<int>(ExplosionType::EXTINGUISHER), "Extinguisher"},
	    {static_cast<int>(ExplosionType::PROGRAMMABLEAR), "Programmable AR"},
	    {static_cast<int>(ExplosionType::TRAIN), "Train"},
	    {static_cast<int>(ExplosionType::BARREL), "Barrel"},
	    {static_cast<int>(ExplosionType::PROPANE), "Propane"},
	    {static_cast<int>(ExplosionType::BLIMP), "Blimp"},
	    {static_cast<int>(ExplosionType::DIR_FLAME_EXPLODE), "Flame Explode"},
	    {static_cast<int>(ExplosionType::TANKER), "Tanker"},
	    {static_cast<int>(ExplosionType::PLANE_ROCKET), "Plane Rocket"},
	    {static_cast<int>(ExplosionType::VEHICLE_BULLET), "Vehicle Bullet"},
	    {static_cast<int>(ExplosionType::GAS_TANK), "Gas Tank"},
	    {static_cast<int>(ExplosionType::BIRD_CRAP), "Bird Crap"},
	    {static_cast<int>(ExplosionType::RAILGUN), "Railgun"},
	    {static_cast<int>(ExplosionType::BLIMP2), "Blimp 2"},
	    {static_cast<int>(ExplosionType::FIREWORK), "Firework"},
	    {static_cast<int>(ExplosionType::SNOWBALL), "Snowball"},
	    {static_cast<int>(ExplosionType::PROXMINE), "Proxmine"},
	    {static_cast<int>(ExplosionType::VALKYRIE_CANNON), "Valkyrie Cannon"},
	    {static_cast<int>(ExplosionType::AIR_DEFENCE), "Air Defence"},
	    {static_cast<int>(ExplosionType::PIPEBOMB), "Pipebomb"},
	    {static_cast<int>(ExplosionType::VEHICLEMINE), "Vehicle Mine"},
	    {static_cast<int>(ExplosionType::EXPLOSIVEAMMO), "Explosive Ammo"},
	    {static_cast<int>(ExplosionType::APCSHELL), "APC Shell"},
	    {static_cast<int>(ExplosionType::BOMB_CLUSTER), "Cluster Bomb"},
	    {static_cast<int>(ExplosionType::BOMB_GAS), "Gas Bomb"},
	    {static_cast<int>(ExplosionType::BOMB_INCENDIARY), "Incendiary Bomb"},
	    {static_cast<int>(ExplosionType::BOMB_STANDARD), "Standard Bomb"},
	    {static_cast<int>(ExplosionType::TORPEDO), "Torpedo"},
	    {static_cast<int>(ExplosionType::TORPEDO_UNDERWATER), "Underwater Torpedo"},
	    {static_cast<int>(ExplosionType::BOMBUSHKA_CANNON), "Bombushka Cannon"},
	    {static_cast<int>(ExplosionType::BOMB_CLUSTER_SECONDARY), "Secondary Cluster Bomb"},
	    {static_cast<int>(ExplosionType::HUNTER_BARRAGE), "Hunter Barrage"},
	    {static_cast<int>(ExplosionType::HUNTER_CANNON), "Hunter Cannon"},
	    {static_cast<int>(ExplosionType::ROGUE_CANNON), "Rogue Cannon"},
	    {static_cast<int>(ExplosionType::MINE_UNDERWATER), "Underwater Mine"},
	    {static_cast<int>(ExplosionType::ORBITAL_CANNON), "Orbital Cannon"},
	    {static_cast<int>(ExplosionType::BOMB_STANDARD_WIDE), "Wide Standard Bomb"},
	    {static_cast<int>(ExplosionType::EXPLOSIVEAMMO_SHOTGUN), "Explosive Shotgun Ammo"},
	    {static_cast<int>(ExplosionType::OPPRESSOR2_CANNON), "Oppressor Mk II Cannon"},
	    {static_cast<int>(ExplosionType::MORTAR_KINETIC), "Kinetic Mortar"},
	    {static_cast<int>(ExplosionType::VEHICLEMINE_KINETIC), "Kinetic Vehicle Mine"},
	    {static_cast<int>(ExplosionType::VEHICLEMINE_EMP), "EMP Vehicle Mine"},
	    {static_cast<int>(ExplosionType::VEHICLEMINE_SPIKE), "Spike Vehicle Mine"},
	    {static_cast<int>(ExplosionType::VEHICLEMINE_SLICK), "Slick Vehicle Mine"},
	    {static_cast<int>(ExplosionType::VEHICLEMINE_TAR), "Tar Vehicle Mine"},
	    {static_cast<int>(ExplosionType::SCRIPT_DRONE), "Script Drone"},
	    {static_cast<int>(ExplosionType::RAYGUN), "Ray Gun"},
	    {static_cast<int>(ExplosionType::BURIEDMINE), "Buried Mine"},
	    {static_cast<int>(ExplosionType::SCRIPT_MISSILE), "Script Missile"},
	    {static_cast<int>(ExplosionType::RCTANK_ROCKET), "RC Tank Rocket"},
	    {static_cast<int>(ExplosionType::BOMB_WATER), "Water Bomb"},
	    {static_cast<int>(ExplosionType::BOMB_WATER_SECONDARY), "Secondary Water Bomb"},
	    {static_cast<int>(ExplosionType::_0xF728C4A9), "Unknown 0xF728C4A9"},
	    {static_cast<int>(ExplosionType::_0xBAEC056F), "Unknown 0xBAEC056F"},
	    {static_cast<int>(ExplosionType::FLASHGRENADE), "Flash Grenade"},
	    {static_cast<int>(ExplosionType::STUNGRENADE), "Stun Grenade"},
	    {static_cast<int>(ExplosionType::_0x763D3B3B), "Unknown 0x763D3B3B"},
	    {static_cast<int>(ExplosionType::SCRIPT_MISSILE_LARGE), "Large Script Missile"},
	    {static_cast<int>(ExplosionType::SUBMARINE_BIG), "Submarine Big"},
	    {static_cast<int>(ExplosionType::EMPLAUNCHER_EMP), "EMP Launcher"},
	};

	static ListCommand _SelectedExplosion{
	    "selectedexplosion",
	    "Explosion Type",
	    "Select an explosion type",
	    explosionTypes,
	    static_cast<int>(ExplosionType::BULLET) // default to Bullet
	};

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
