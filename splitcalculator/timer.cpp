#include "timer.h"

Timer::Timer(): current_frame(0), game_version(Version(executable_version)), level_flags(Flag()),
game_handle(get_handle(executable, executable_class)), timer(clock()), current_value(0), split(Split()),
addon_time(0), reset_value(0), save_time(false), timer_on(false) {

}

std::string Timer::update_data() {
    if (current_frame == update_speed) {
        current_frame = 0;

        game_handle = get_handle(executable, executable_class);

        if (game_handle) {
            //executable_version = get_executable_version(game_handle);

            uint64_t main_address = get_handle_address(game_handle);

            // anti-cheat must be turned off to find game module
            HMODULE halo1_module = get_dll_hmodule(game_handle, L"halo1.dll");
            HMODULE halo2_module = get_dll_hmodule(game_handle, L"halo2.dll");
            HMODULE halo3_module = get_dll_hmodule(game_handle, L"halo3.dll");
            HMODULE halo3odst_module = get_dll_hmodule(game_handle, L"halo3odst.dll");
            HMODULE haloreach_module = get_dll_hmodule(game_handle, L"haloreach.dll");
            HMODULE halo4_module = get_dll_hmodule(game_handle, L"halo4.dll");

            uint64_t halo1_dll = (uint64_t)halo1_module;
            uint64_t halo2_dll = (uint64_t)halo2_module;
            uint64_t halo3_dll = (uint64_t)halo3_module;
            uint64_t halo3odst_dll = (uint64_t)halo3odst_module;
            uint64_t haloreach_dll = (uint64_t)haloreach_module;
            uint64_t halo4_dll = (uint64_t)halo4_module;

            // assuming these two values do not change depend on the game, since they are both based on the main address
            // 129 represents pause menu, 255 represents in-game, 57 represents postgame carnage report, 44 represents loading screen
            uint64_t menu_state = main_address + game_version.menu_state_offset;
            // 0 represents game lobby, 1 represents in game
            uint64_t screen_state = main_address + game_version.screen_state_offset;

            int menu_value = read_int_byte_from_memory(game_handle, menu_state);
            int screen_value = read_int_byte_from_memory(game_handle, screen_state);

            // start timer if player enters a loading screen
            if (menu_value == 1 && (screen_value == 44 || screen_value == 129 || screen_value == 255))
                timer_on = true;

            if (timer_on) {
                split = Split();
                save_time = false;

                current_value = clock() - reset_value + addon_time;

                // stop timer if player reaches the campaign menu
                if (menu_value == 0)
                    timer_on = false;

                // todo: stop timer if player restarts on the first level
                // todo: finish flags for h3, h3odst, hr, h4

                // stop timer if player finishes the final level
                // coordinate value truncation sucks, will fix later
                if (read_string_from_memory(game_handle, halo1_dll + game_version.h1_level_offset, 3).compare(
                    level_flags.h1_level_tags[level_flags.h1_level_tags.size() - 1]) == 0 &&
                    read_int_byte_from_memory(game_handle, halo1_dll + game_version.h1_bsp_offset) == 7 &&
                    (int)read_float_from_memory(game_handle, halo1_dll + game_version.h1_xpos_offset) == 1062 &&
                    (int)read_float_from_memory(game_handle, halo1_dll + game_version.h1_ypos_offset) == -3 &&
                    (int)read_float_from_memory(game_handle, halo1_dll + game_version.h1_zpos_offset) == 35)
                    timer_on = false;

                if (read_string_from_memory(game_handle, halo2_dll + game_version.h2_level_offset, 3).compare(
                    level_flags.h2_level_tags[level_flags.h2_level_tags.size() - 1]) == 0 &&
                    read_int_byte_from_memory(game_handle, halo2_dll + game_version.h2_bsp_offset) == 3 &&
                    (int)read_float_from_memory(game_handle, halo2_dll + game_version.h2_xpos_offset) == -16 &&
                    (int)read_float_from_memory(game_handle, halo2_dll + game_version.h2_ypos_offset) == 165 &&
                    (int)read_float_from_memory(game_handle, halo2_dll + game_version.h2_zpos_offset) == 24)
                    timer_on = false;

                if (read_string_from_memory(game_handle, halo3_dll + game_version.h3_level_offset, NULL).compare("120") == 0 &&
                    read_ulong_from_memory(game_handle, halo3_dll + game_version.h3_bsp_offset) == 1219770714111)
                    timer_on = false;

                if (read_string_from_memory(game_handle, halo3odst_dll + game_version.h3odst_level_offset, NULL).compare("1300") == 0 &&
                    read_uint_from_memory(game_handle, halo3odst_dll + game_version.h3odst_bsp_offset) == 112)
                    timer_on = false;

                if (read_string_from_memory(game_handle, haloreach_dll + game_version.hr_level_offset, NULL).compare("m70") == 0 &&
                    read_uint_from_memory(game_handle, haloreach_dll + game_version.hr_bsp_offset) == 2047)
                    timer_on = false;

                if (read_string_from_memory(game_handle, halo4_dll + game_version.h4_level_offset, NULL).compare("m90") == 0 &&
                    read_ulong_from_memory(game_handle, halo4_dll + game_version.h4_bsp_offset) == 0x0000000000A00006)
                    timer_on = false;

                // only h1 does not pause at postgame carnage report
                if (screen_value == 129 || screen_value == 255 || (screen_value == 57 && !halo1_module)) {
                    save_time = true;
                } else if (screen_value == 44) {
                    if (save_time) {
                        // save current split as a subsplit
                        split.save();

                        // pause timer math
                        addon_time = clock() - reset_value + addon_time;
                        save_time = false;
                    }

                    reset_value = clock();
                }

                split.update(convert_milliseconds(current_value));

                if (test_output)
                    std::cout << split.main_split << std::endl;

                return split.main_split;
            } else {
                if (test_output)
                    std::cout << convert_milliseconds(current_value) << std::endl;

                return convert_milliseconds(current_value);
            }
        }
    }
    else {
        current_frame++;
    }

    return convert_milliseconds(current_value);
}

Timer::~Timer() {
    if (game_handle)
        CloseHandle(game_handle);
}

static HANDLE get_handle(const LPCSTR & lpWindowName, const LPCSTR & lpClassName) {
    HWND hWnd = FindWindowA(lpClassName, lpWindowName);
    DWORD pid;
    GetWindowThreadProcessId(hWnd, &pid);
    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    return handle;
}

static void get_executable_version(const HANDLE & handle)
{

}

static uint64_t get_handle_address(const HANDLE & handle) {
    HMODULE hModules[1024];
    DWORD cbNeeded;

    if (EnumProcessModules(handle, hModules, sizeof(hModules), &cbNeeded))
        return (uint64_t)hModules[0];

    return 0;
}

static HMODULE get_dll_hmodule(const HANDLE & handle, const std::wstring & moduleName) {
    HMODULE hModules[1024];
    DWORD cbNeeded;

    if (EnumProcessModules(handle, hModules, sizeof(hModules), &cbNeeded)) {
        for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
            TCHAR modName[MAX_PATH];

            if (GetModuleFileNameEx(handle, hModules[i], modName, sizeof(modName) / sizeof(TCHAR))) {
                std::wstring wstrModName = modName;
                std::wstring wstrModContain = moduleName;

                if (wstrModName.find(wstrModContain) != std::string::npos)
                    return hModules[i];
            }
        }
    }

    return NULL;
}

static std::string read_string_from_memory(const HANDLE & handle, const uint64_t & address, const int & length) {
    // set length to NULL if you want to read string until null terminator
    int offset = 0;
    char c = '\0';
    std::string s = "";

    if (length) {
        do {
            ReadProcessMemory(handle, (void*)(address + offset), &c, sizeof(char), NULL);
            s += c;

            ++offset;
        } while (offset < length);
    }
    else {
        do {
            ReadProcessMemory(handle, (void*)(address + offset), &c, sizeof(char), NULL);
            s += c;

            ++offset;
        } while (c);
    }

    return s;
}

static int read_int_byte_from_memory(const HANDLE & handle, const uint64_t & address) {
    byte b = 0;
    ReadProcessMemory(handle, (void*)address, &b, sizeof(byte), NULL);

    return (int)b;
}

static float read_float_from_memory(const HANDLE & handle, const uint64_t & address) {
    float f = 0.0;
    ReadProcessMemory(handle, (void*)address, &f, sizeof(float), NULL);

    return f;
}

static unsigned long read_ulong_from_memory(const HANDLE & handle, const uint64_t & address) {
    unsigned long ul = 0;
    ReadProcessMemory(handle, (void*)address, &ul, sizeof(unsigned long), NULL);

    return ul;
}

static unsigned int read_uint_from_memory(const HANDLE & handle, const uint64_t & address) {
    unsigned int ui = 0;
    ReadProcessMemory(handle, (void*)address, &ui, sizeof(unsigned int), NULL);

    return ui;
}

static std::string convert_milliseconds(const int & clock_ticks) {
    int ms = clock_ticks / (CLOCKS_PER_SEC / 1000);

    int milliseconds = ms % 1000;
    int seconds = (int)((ms / 1000) % 60);
    int minutes = (int)((ms / (1000 * 60)) % 60);
    int hours = (int)(ms / (1000 * 60 * 60));

    // safety for if tick subtraction does not fully equal 0
    if (milliseconds <= 10)
        milliseconds = 0;

    std::stringstream s;
    std::stringstream t;
    std::stringstream u;
    std::stringstream v;

    s << std::setw(3) << std::setfill('0') << std::to_string(milliseconds);
    t << std::setw(2) << std::setfill('0') << std::to_string(seconds);
    u << std::setw(2) << std::setfill('0') << std::to_string(minutes);
    v << std::setw(2) << std::setfill('0') << std::to_string(hours);

    return v.str() + ':' + u.str() + ':' + t.str() + '.' + s.str();
}