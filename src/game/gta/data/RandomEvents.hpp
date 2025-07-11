#pragma once
#include "core/util/Joaat.hpp"

namespace YimMenu
{
	static constexpr auto randomEventNames = std::to_array({"Drug Vehicle",
	    "Movie Props",
	    "Sleeping Guard",
	    "Exotic Exports",
	    "The Slashers",
	    "Phantom Car",
	    "Sightseeing",
	    "Smuggler Trail",
	    "Cerberus Surprise",
	    "Smuggler Plane",
	    "Crime Scene",
	    "Metal Detector",
	    "Finders Keepers",
	    "Shop Robbery",
	    "The Gooch",
	    "Weazel Plaza Shootout",
	    "Armored Truck",
	    "Possessed Animals",
	    "Ghosts Exposed",
	    "Happy Holidays Hauler",
	    "Community Outreach"});

	static constexpr auto randomEventScripts = std::to_array({"fm_content_drug_vehicle"_J,
	    "fm_content_movie_props"_J,
	    "fm_content_golden_gun"_J,
	    "fm_content_vehicle_list"_J,
	    "fm_content_slasher"_J,
	    "fm_content_phantom_car"_J,
	    "fm_content_sightseeing"_J,
	    "fm_content_smuggler_trail"_J,
	    "fm_content_cerberus"_J,
	    "fm_content_smuggler_plane"_J,
	    "fm_content_crime_scene"_J,
	    "fm_content_metal_detector"_J,
	    "fm_content_convoy"_J,
	    "fm_content_robbery"_J,
	    "fm_content_xmas_mugger"_J,
	    "fm_content_bank_shootout"_J,
	    "fm_content_armoured_truck"_J,
	    "fm_content_possessed_animals"_J,
	    "fm_content_ghosthunt"_J,
	    "fm_content_xmas_truck"_J,
	    "fm_content_community_outreach"_J});

	static constexpr auto randomEventCooldowns = std::to_array({
	    "SUM22_RE_DRUG_VEHICLE_INACTIVE_TIME"_J,
	    "SUM22_RE_MOVIE_PROPS_INACTIVE_TIME"_J,
	    "SUM22_RE_GOLDEN_GUN_INACTIVE_TIME"_J,
	    "SUM22_RE_VEHICLE_LIST_INACTIVE_TIME"_J,
	    "STANDARDCONTROLLERVOLUME_COOLDOWN"_J,
	    "STANDARDTARGETTINGTIME_COOLDOWN"_J,
	    "SSP2_COOLDOWN"_J
	    "SUM22_RE_SMUGGLER_TRAIL_INACTIVE_TIME"_J,
	    "NC_SOURCE_TRUCK_COOLDOWN"_J,
	    "SUM22_RE_SMUGGLER_PLANE_INACTIVE_TIME"_J,
	    "SUM22_RE_CRIME_SCENE_INACTIVE_TIME"_J,
	    "SUM22_RE_METAL_DETECTOR_INACTIVE_TIME"_J,
	    "XM22_RE_GANG_CONVOY_INACTIVE_TIME"_J,
	    "XM22_RE_ROBBERY_INACTIVE_TIME"_J,
	    "STANDARD_KEYBIND_COOLDOWN"_J,
	    "XM22_RE_BANK_SHOOTOUT_INACTIVE_TIME"_J,
	    (joaat_t)NULL,
	    "STANDARDCONTROLLERVOLUME_COOLDOWN"_J,
	    "SUM23_RE_GHOSTHUNT_INACTIVE_TIME"_J,
	    "XMAS_TRUCK_INACTIVE_TIME"_J,
	    "RE_COMMUNITY_OUTREACH_INACTIVE_TIME"_J,
	});

	static constexpr auto randomEventAvailabilities = std::to_array({
	    "SUM22_RE_DRUG_VEHICLE_AVAILABLE_TIME"_J,
	    "SUM22_RE_MOVIE_PROPS_AVAILABLE_TIME"_J,
	    "SUM22_RE_GOLDEN_GUN_AVAILABLE_TIME"_J,
	    "SUM22_RE_VEHICLE_LIST_AVAILABLE_TIME"_J,
	    "STANDARDCONTROLLERVOLUME_AVAILABILITY"_J,
	    "STANDARDTARGETTINGTIME_AVAILABILITY"_J,
	    "SSP2_AVAILABILITY"_J,
	    "SUM22_RE_SMUGGLER_TRAIL_AVAILABLE_TIME"_J,
	    "NC_SOURCE_TRUCK_AVAILABILITY"_J,
	    "SUM22_RE_SMUGGLER_PLANE_AVAILABLE_TIME"_J,
	    "SUM22_RE_CRIME_SCENE_AVAILABLE_TIME"_J,
	    "SUM22_RE_METAL_DETECTOR_AVAILABLE_TIME"_J,
	    "XM22_RE_GANG_CONVOY_AVAILABLE_TIME"_J,
	    "XM22_RE_ROBBERY_AVAILABLE_TIME"_J,
	    "STANDARD_KEYBIND_AVAILABILITY"_J,
	    "XM22_RE_BANK_SHOOTOUT_AVAILABLE_TIME"_J,
	    (joaat_t)NULL,
	    "STANDARDCONTROLLERVOLUME_AVAILABILITY"_J,
	    "SUM23_RE_GHOSTHUNT_AVAILABLE_TIME"_J,
	    "XMAS_TRUCK_AVAILABLE_TIME"_J,
	    "RE_COMMUNITY_OUTREACH_AVAILABLE_TIME"_J,
	});
}