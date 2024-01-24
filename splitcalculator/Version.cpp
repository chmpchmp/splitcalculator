#include "Version.h"

Version::Version(string version)
{
	int c = 0;

	if (version == "1.3272.0.0")
		c = 1;

	switch (c)
	{
		case 1:
			// these offsets should be applied to the address of the main executable
			menu_state_offset = 0x3EC79A9;
			screen_state_offset = 0x3FBB499;

			// each offset should be added to their respective dll addresses
			// current level offsets
			h1_level_offset = 0x2B22764;
			h2_level_offset = 0xE6FE68;
			h3_level_offset = 0x20A8118;
			h3odst_level_offset = 0x20EF128;
			hr_level_offset = 0x2A1F597;
			h4_level_offset = 0x2AFF81F;

			// bsp offsets
			h1_bsp_offset = 0x1B860A4;
			h2_bsp_offset = 0xDF8D74;
			h3_bsp_offset = 0xA4E170;
			h3odst_bsp_offset = 0x46E261C;
			hr_bsp_offset = 0x4E2FBA8;
			h4_bsp_offset = 0x275D550;

			// in-game time offsets
			h1_igt_offset = 0x2EA31C4;

			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		default:
			throw "version " + version + " is not supported";
	}
};