#pragma once

#include <string>

using namespace std;

class Version
{
public:
	// these offsets should be applied to the address of the main executable
	unsigned long long menu_state_offset;
	unsigned long long screen_state_offset;

	// current level offsets
	// each offset should be added to their respective dll addresses
	unsigned long long h1_level_offset;
	unsigned long long h2_level_offset;
	unsigned long long h3_level_offset;
	unsigned long long h3odst_level_offset;
	unsigned long long hr_level_offset;
	unsigned long long h4_level_offset;

	// each offset should be added to their respective dll addresses
	unsigned long long h1_bsp_offset;
	unsigned long long h2_bsp_offset;
	unsigned long long h3_bsp_offset;
	unsigned long long h3odst_bsp_offset;
	unsigned long long hr_bsp_offset;
	unsigned long long h4_bsp_offset;

	unsigned long long h1_igt_offset;
	unsigned long long h2_igt_offset;
	unsigned long long h3_igt_offset;
	unsigned long long h3odst_igt_offset;
	unsigned long long hr_igt_offset;
	unsigned long long h4_igt_offset;

	Version(string version);
};

