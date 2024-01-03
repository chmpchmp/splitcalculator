// working currently for the Steam release of Halo: The Master Chief Collection version 1.3272.0.0

#include "Split.h"

#include <Windows.h>
#include <psapi.h>
#include <libloaderapi.h>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

using namespace std;

// two random spaces is correct for whatever reason
static LPCSTR executable = "Halo: The Master Chief Collection  ";
static LPCSTR executable_class = "UnrealWindow";

const static HANDLE get_handle(const LPCSTR & lpWindowName, const LPCSTR & lpClassName) {
    HWND hWnd = FindWindowA(lpClassName, lpWindowName);
    DWORD pid;
    GetWindowThreadProcessId(hWnd, &pid);
    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    return handle;
}

const static unsigned long long get_handle_address(const HANDLE & handle) {
    HMODULE hModules[1024];
    DWORD cbNeeded;

    if (EnumProcessModules(handle, hModules, sizeof(hModules), &cbNeeded))
    {
        return (unsigned long long)hModules[0];
    }

    return NULL;
}

const static HMODULE get_dll_hmodule(const HANDLE & handle, const wstring & moduleName) {
    HMODULE hModules[1024];
    DWORD cbNeeded;

    if (EnumProcessModules(handle, hModules, sizeof(hModules), &cbNeeded))
    {
        for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
        {
            TCHAR szModName[MAX_PATH];

            if (GetModuleFileNameEx(handle, hModules[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
            {
                wstring wstrModName = szModName;
                wstring wstrModContain = moduleName;

                if (wstrModName.find(wstrModContain) != string::npos)
                    return hModules[i];
            }
        }
    }

    return NULL;
}

const static int read_int_byte_from_memory(const HANDLE & handle, const unsigned long long & address) {
    byte b = 0;
    ReadProcessMemory(handle, (void*)address, &b, sizeof(byte), NULL);

    return (int)b;
}

const static string read_string_from_memory(const HANDLE & handle, const unsigned long long & address, const int & length = NULL) {
    int offset = 0;
    char c = '\0';
    string s = "";

    if (length) {
        do {
            ReadProcessMemory(handle, (void*)(address + offset), &c, sizeof(char), NULL);
            s += c;

            ++offset;
        } while (offset < length);
    } else {
        do {
            ReadProcessMemory(handle, (void*)(address + offset), &c, sizeof(char), NULL);
            s += c;

            ++offset;
        } while (c);
    }

    return s;
}

const static string convert_milliseconds(const int & clock_ticks) {
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

int main() {
    while (true) {
        HANDLE handle = get_handle(executable, executable_class);

        if (handle) {
            unsigned long long main_address = get_handle_address(handle);

            // anti-cheat must be turned off to find halo2 module
            HMODULE halo2_module = get_dll_hmodule(handle, L"halo2.dll");

            if (halo2_module) {
                unsigned long long halo2_dll = (unsigned long long)halo2_module;

                unsigned long long menu_state = main_address + 0x3EC79A9;
                unsigned long long screen_state = main_address + 0x3FBB499;

                Split split = Split();

                int addon_time = 0;
                int reset_value = 0;
                bool save_time = false;
                bool timer_on = false;
                clock_t timer = clock();

                while (true) {
                    int menu_value = read_int_byte_from_memory(handle, menu_state);
                    int screen_value = read_int_byte_from_memory(handle, screen_state);

                    string s = read_string_from_memory(handle, halo2_dll + 0xE6FE68, 3);
                    string t = read_string_from_memory(handle, halo2_dll + 0xE6FE97, 3);
                    string u = read_string_from_memory(handle, halo2_dll + 0x159FB1B, 3);
                    string v = read_string_from_memory(handle, halo2_dll + 0x15A144B, 3);

                    if (menu_value != 0)
                        timer_on = true;

                    if (timer_on) {
                        // 0 represents player is sitting in menu
                        if (menu_value == 0) {
                            break;

                        }
                        // 129 represents pause screen, 255 represents in-game
                        else if (screen_value == 129 || screen_value == 255) {
                            save_time = true;

                        }
                        // 44 represents loading screen
                        else if (screen_value == 44) {
                            if (save_time) {
                                split.update(s);
                                addon_time = clock() - reset_value + addon_time;
                                save_time = false;
                            }

                            reset_value = clock();
                        }

                        split.increment(convert_milliseconds(clock() - reset_value + addon_time));
                    }

                    cout << split.main_split << endl;
                }
            }

            CloseHandle(handle);
        } else {
            cout << "Window could not be found" << endl;
        }
    }
}