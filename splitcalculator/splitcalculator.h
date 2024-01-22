#pragma once

#include <Windows.h>
#include <psapi.h>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

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