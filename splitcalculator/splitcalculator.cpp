// working currently for the Steam release of Halo: The Master Chief Collection version 1.3272.0.0

#include "splitcalculator.h"
#include "Split.h"
#include "Version.h"
#include "Flag.h"

#include <ctime>
#include <iostream>

using namespace std;

bool debug_output = false;

// two random spaces is correct for whatever reason
static LPCSTR executable = "Halo: The Master Chief Collection  ";
static LPCSTR executable_class = "UnrealWindow";
static string executable_version = "1.3272.0.0";

static HANDLE get_handle(const LPCSTR & lpWindowName, const LPCSTR & lpClassName);
static void get_executable_version(const HANDLE & handle);
static uint64_t get_handle_address(const HANDLE & handle);
static HMODULE get_dll_hmodule(const HANDLE & handle, const wstring& moduleName);
static string read_string_from_memory(const HANDLE & handle, const uint64_t & address, const int & length);
static int read_int_byte_from_memory(const HANDLE & handle, const uint64_t & address);
static float read_float_from_memory(const HANDLE & handle, const uint64_t & address);
static unsigned long read_ulong_from_memory(const HANDLE & handle, const uint64_t & address);
static unsigned int read_uint_from_memory(const HANDLE & handle, const uint64_t & address);
static string convert_milliseconds(const int & clock_ticks);

int main()
{
    if (debug_output) {
        cout << "Running splitcalculator for Halo: The Master Chief Collection version " << executable_version << endl;
        cout << endl;
        cout << "The timer may break if the build of the game does not match the version above" << endl;
    }

    while (true)
    {
        HANDLE handle = get_handle(executable, executable_class);
        get_executable_version(handle);

        Version version = Version(executable_version);
        Flag flag = Flag();

        if (handle)
        {
            uint64_t main_address = get_handle_address(handle);

            // anti-cheat must be turned off to find game module
            HMODULE halo1_module = get_dll_hmodule(handle, L"halo1.dll");
            HMODULE halo2_module = get_dll_hmodule(handle, L"halo2.dll");
            HMODULE halo3_module = get_dll_hmodule(handle, L"halo3.dll");
            HMODULE halo3odst_module = get_dll_hmodule(handle, L"halo3odst.dll");
            HMODULE haloreach_module = get_dll_hmodule(handle, L"haloreach.dll");
            HMODULE halo4_module = get_dll_hmodule(handle, L"halo4.dll");

            uint64_t halo1_dll = (uint64_t)halo1_module;
            uint64_t halo2_dll = (uint64_t)halo2_module;
            uint64_t halo3_dll = (uint64_t)halo3_module;
            uint64_t halo3odst_dll = (uint64_t)halo3odst_module;
            uint64_t haloreach_dll = (uint64_t)haloreach_module;
            uint64_t halo4_dll = (uint64_t)halo4_module;

            // assuming these two values do not change depend on the game, since they are both based on the main address
            uint64_t menu_state = main_address + version.menu_state_offset;
            uint64_t screen_state = main_address + version.screen_state_offset;

            // separate handle variable to check whether the game has been closed
            HANDLE active_handle = get_handle(executable, executable_class);

            // what the timer should display if the timer is not actively tracking in-game data
            // this value should be the time of when the player exited into the menu after the first use
            int starting_value = 0;

            while (active_handle)
            {
                clock_t timer = clock();
                Split split = Split();

                int addon_time = 0;
                int reset_value = 0;
                bool save_time = false;
                bool timer_on = false;

                while (active_handle)
                {
                    // find better way to check and manage active handle
                    CloseHandle(active_handle);
                    active_handle = get_handle(executable, executable_class);

                    int menu_value = read_int_byte_from_memory(handle, menu_state);
                    int screen_value = read_int_byte_from_memory(handle, screen_state);

                    string s;

                    // start timer if player enters a loading screen
                    if (menu_value != 0 && (screen_value == 44 || screen_value == 129 || screen_value == 255))
                        timer_on = true;

                    if (timer_on)
                    {
                        starting_value = clock() - reset_value + addon_time;

                        // 0 represents player is sitting in menu, exit timer
                        if (menu_value == 0)
                            break;

                        // coordinate value truncation sucks, will fix later
                        if (read_string_from_memory(handle, halo1_dll + version.h1_level_offset, 3).compare(flag.h1_level_tags[flag.h1_level_tags.size() - 1]) == 0 &&
                            read_int_byte_from_memory(handle, halo1_dll + version.h1_bsp_offset) == 7 &&
                            (int)read_float_from_memory(handle, halo1_dll + version.h1_xpos_offset) == 1062 &&
                            (int)read_float_from_memory(handle, halo1_dll + version.h1_ypos_offset) == -3 &&
                            (int)read_float_from_memory(handle, halo1_dll + version.h1_zpos_offset) == 35)
                            break;

                        if (read_string_from_memory(handle, halo2_dll + version.h2_level_offset, 3).compare(flag.h2_level_tags[flag.h2_level_tags.size() - 1]) == 0 &&
                            read_int_byte_from_memory(handle, halo2_dll + version.h2_bsp_offset) == 3 &&
                            (int)read_float_from_memory(handle, halo2_dll + version.h2_xpos_offset) == -16 &&
                            (int)read_float_from_memory(handle, halo2_dll + version.h2_ypos_offset) == 165 &&
                            (int)read_float_from_memory(handle, halo2_dll + version.h2_zpos_offset) == 24)
                            break;

                        if (read_string_from_memory(handle, halo3_dll + version.h3_level_offset).compare("120") == 0 &&
                            read_ulong_from_memory(handle, halo3_dll + version.h3_bsp_offset) == 1219770714111)
                            break;

                        if (read_string_from_memory(handle, halo3odst_dll + version.h3odst_level_offset).compare("1300") == 0 &&
                            read_uint_from_memory(handle, halo3odst_dll + version.h3odst_bsp_offset) == 112)
                            break;

                        if (read_string_from_memory(handle, haloreach_dll + version.hr_level_offset).compare("m70") == 0 &&
                            read_uint_from_memory(handle, haloreach_dll + version.hr_bsp_offset) == 2047)
                            break;

                        if (read_string_from_memory(handle, halo4_dll + version.h4_level_offset).compare("m90") == 0 &&
                            read_ulong_from_memory(handle, halo4_dll + version.h4_bsp_offset) == 0x0000000000A00006)
                            break;

                        // only h1 does not pause at postgame carnage report
                        // 129 represents pause menu, 255 represents in-game, 57 represents postgame carnage report
                        //if (screen_value == 129 || screen_value == 255 || screen_value == 57)
                        if (screen_value == 129 || screen_value == 255)
                        {
                            save_time = true;
                        }
                        // 44 represents loading screen
                        else if (screen_value == 44)
                        {
                            if (save_time) {
                                split.save();

                                addon_time = clock() - reset_value + addon_time;
                                save_time = false;
                            }

                            reset_value = clock();
                        }

                        split.update(convert_milliseconds(starting_value));

                        if (debug_output)
                            cout << split.main_split << endl;
                    }
                    else
                    {
                        if (debug_output)
                            cout << convert_milliseconds(starting_value) << endl;
                    }
                }
            }

            CloseHandle(handle);
        }
    }
}