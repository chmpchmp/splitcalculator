// working currently for the Steam release of Halo: The Master Chief Collection version 1.3272.0.0

#include "Split.h"
#include "Version.h"

#include <Windows.h>
#include <psapi.h>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

using namespace std;

// two random spaces is correct for whatever reason
static LPCSTR executable = "Halo: The Master Chief Collection  ";
static LPCSTR executable_class = "UnrealWindow";
static string executable_version = "1.3272.0.0";

static HANDLE get_handle(const LPCSTR & lpWindowName, const LPCSTR & lpClassName)
{
    HWND hWnd = FindWindowA(lpClassName, lpWindowName);
    DWORD pid;
    GetWindowThreadProcessId(hWnd, &pid);
    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    return handle;
}

static unsigned long long get_handle_address(const HANDLE & handle)
{
    HMODULE hModules[1024];
    DWORD cbNeeded;

    if (EnumProcessModules(handle, hModules, sizeof(hModules), &cbNeeded))
        return (unsigned long long)hModules[0];

    return 0;
}

static HMODULE get_dll_hmodule(const HANDLE & handle, const wstring & moduleName)
{
    HMODULE hModules[1024];
    DWORD cbNeeded;

    if (EnumProcessModules(handle, hModules, sizeof(hModules), &cbNeeded))
    {
        for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
        {
            TCHAR modName[MAX_PATH];

            if (GetModuleFileNameEx(handle, hModules[i], modName, sizeof(modName) / sizeof(TCHAR)))
            {
                wstring wstrModName = modName;
                wstring wstrModContain = moduleName;

                if (wstrModName.find(wstrModContain) != string::npos)
                    return hModules[i];
            }
        }
    }

    return NULL;
}

static string read_string_from_memory(const HANDLE & handle, const unsigned long long & address, const int & length = NULL)
{
    int offset = 0;
    char c = '\0';
    string s = "";

    if (length)
    {
        do
        {
            ReadProcessMemory(handle, (void*)(address + offset), &c, sizeof(char), NULL);
            s += c;

            ++offset;
        }
        while (offset < length);
    }
    else
    {
        do
        {
            ReadProcessMemory(handle, (void*)(address + offset), &c, sizeof(char), NULL);
            s += c;

            ++offset;
        }
        while (c);
    }

    return s;
}

static int read_int_byte_from_memory(const HANDLE & handle, const unsigned long long & address)
{
    byte b = 0;
    ReadProcessMemory(handle, (void*)address, &b, sizeof(byte), NULL);

    return (int)b;
}

static unsigned long read_ulong_from_memory(const HANDLE & handle, const unsigned long long & address)
{
    unsigned long ul = 0;
    ReadProcessMemory(handle, (void*)address, &ul, sizeof(unsigned long), NULL);

    return ul;
}

static unsigned int read_uint_from_memory(const HANDLE & handle, const unsigned long long & address)
{
    unsigned int ui = 0;
    ReadProcessMemory(handle, (void*)address, &ui, sizeof(unsigned int), NULL);

    return ui;
}

static string convert_milliseconds(const int & clock_ticks)
{
    int ms = clock_ticks / (CLOCKS_PER_SEC / 1000);

    int milliseconds = ms % 1000;
    int seconds = (int)((ms / 1000) % 60);
    int minutes = (int)((ms / (1000 * 60)) % 60);
    int hours = (int)(ms / (1000 * 60 * 60));

    stringstream s;
    stringstream t;
    stringstream u;
    stringstream v;

    s << setw(3) << setfill('0') << to_string(milliseconds);
    t << setw(2) << setfill('0') << to_string(seconds);
    u << setw(2) << setfill('0') << to_string(minutes);
    v << setw(2) << setfill('0') << to_string(hours);

    return v.str() + ':' + u.str() + ':' + t.str() + '.' + s.str();
}

int main()
{
    cout << "Running splitcalculator for Halo: The Master Chief Collection version " << executable_version << endl;
    cout << endl;
    cout << "The timer may break if the build of the game does not match the version above" << endl;

    while (true)
    {
        HANDLE handle = get_handle(executable, executable_class);

        Version version = Version(executable_version);

        if (handle)
        {
            unsigned long long main_address = get_handle_address(handle);

            // anti-cheat must be turned off to find game module
            HMODULE halo1_module = get_dll_hmodule(handle, L"halo1.dll");
            HMODULE halo2_module = get_dll_hmodule(handle, L"halo2.dll");
            HMODULE halo3_module = get_dll_hmodule(handle, L"halo3.dll");
            HMODULE halo3odst_module = get_dll_hmodule(handle, L"halo3odst.dll");
            HMODULE haloreach_module = get_dll_hmodule(handle, L"haloreach.dll");
            HMODULE halo4_module = get_dll_hmodule(handle, L"halo4.dll");

            unsigned long long halo1_dll = (unsigned long long)halo1_module;
            unsigned long long halo2_dll = (unsigned long long)halo2_module;
            unsigned long long halo3_dll = (unsigned long long)halo3_module;
            unsigned long long halo3odst_dll = (unsigned long long)halo3odst_module;
            unsigned long long haloreach_dll = (unsigned long long)haloreach_module;
            unsigned long long halo4_dll = (unsigned long long)halo4_module;

            unsigned long long halo1_cs_state = halo1_dll + version.h1_cs_offset;
            unsigned long long halo2_cs_state = halo2_dll + version.h2_cs_offset;
            unsigned long long halo3_cs_state = halo3_dll + version.h3_cs_offset;
            unsigned long long halo3odst_cs_state = halo3odst_dll + version.h3odst_cs_offset;
            unsigned long long haloreach_cs_state = haloreach_dll + version.hr_cs_offset;
            unsigned long long halo4_cs_state = halo4_dll + version.h4_cs_offset;

            // assuming these two values do not change depend on the game, since they are both based on the main address
            unsigned long long menu_state = main_address + version.menu_state_offset;
            unsigned long long screen_state = main_address + version.screen_state_offset;

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
                    CloseHandle(active_handle);
                    active_handle = get_handle(executable, executable_class);

                    int menu_value = read_int_byte_from_memory(handle, menu_state);
                    int screen_value = read_int_byte_from_memory(handle, screen_state);

                    string s;

                    // start timer if player exits from menu
                    if (menu_value != 0)
                        timer_on = true;

                    if (timer_on)
                    {
                        // 0 represents player is sitting in menu, exit timer
                        if (menu_value == 0)
                        {
                            starting_value = clock() - reset_value + addon_time;
                            break;
                        }

                        if (read_string_from_memory(handle, halo1_dll + version.h1_level_offset) == "d40" &&
                            read_int_byte_from_memory(handle, halo1_cs_state) == 7)
                            break;

                        // to do: test this if statement, supposed to stop timer when player reaches last
                        // cutscene of the great journey
                        if (read_string_from_memory(handle, halo2_dll + version.h2_level_offset) == "08b_deltacontrol" &&
                            read_int_byte_from_memory(handle, halo2_cs_state) == 3)
                            break;

                        if (read_string_from_memory(handle, halo3_dll + version.h3_level_offset) == "120" &&
                            read_ulong_from_memory(handle, halo3_cs_state) == 1219770714111)
                            break;

                        if (read_string_from_memory(handle, halo3odst_dll + version.h3odst_level_offset) == "1300" &&
                            read_uint_from_memory(handle, halo3odst_cs_state) == 112)
                            break;

                        if (read_string_from_memory(handle, haloreach_dll + version.hr_level_offset) == "m70" &&
                            read_uint_from_memory(handle, haloreach_cs_state) == 2047)
                            break;

                        if (read_string_from_memory(handle, halo4_dll + version.h4_level_offset) == "m90" &&
                            read_ulong_from_memory(handle, halo4_cs_state) == 0x0000000000A00006)
                            break;

                        // 129 represents pause menu, 255 represents in-game
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

                        split.update(convert_milliseconds(clock() - reset_value + addon_time));

                        cout << split.main_split << endl;
                    }
                    else
                    {
                        cout << convert_milliseconds(starting_value) << endl;
                    }
                }
            }

            CloseHandle(handle);
        }
    }
}