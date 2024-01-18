#pragma once

#include <string>

using namespace std;

class Version
{
public:
	// each offset should be added to their respective dll addresses
	unsigned long long h1_cs_offset;
	unsigned long long h2_cs_offset;
	unsigned long long h3_cs_offset;
	unsigned long long h3odst_cs_offset;
	unsigned long long hr_cs_offset;
	unsigned long long h4_cs_offset;

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

	Version(string version);
};

