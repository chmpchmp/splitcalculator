#pragma once

#include <string>

class Version {
public:
	// these offsets should be applied to the address of the main executable
	uint64_t menu_state_offset;
	uint64_t screen_state_offset;

	// each offset should be added to their respective dll addresses
	uint64_t h1_level_offset;
	uint64_t h2_level_offset;
	uint64_t h3_level_offset;
	uint64_t h3odst_level_offset;
	uint64_t hr_level_offset;
	uint64_t h4_level_offset;

	uint64_t h1_bsp_offset;
	uint64_t h2_bsp_offset;
	uint64_t h3_bsp_offset;
	uint64_t h3odst_bsp_offset;
	uint64_t hr_bsp_offset;
	uint64_t h4_bsp_offset;

	uint64_t h1_xpos_offset;
	uint64_t h1_ypos_offset;
	uint64_t h1_zpos_offset;

	uint64_t h2_xpos_offset;
	uint64_t h2_ypos_offset;
	uint64_t h2_zpos_offset;

	Version();
	Version(std::string version);
};

