#pragma once

#include "split.h"
#include "version.h"
#include "flag.h"

#include <Windows.h>
#include <psapi.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <iostream>

// two random spaces are correct for whatever reason
static LPCSTR executable = "Halo: The Master Chief Collection  ";
static LPCSTR executable_class = "UnrealWindow";
static int update_speed = 50;

class Timer
{
    bool test_output = false;

    int frame;

    HANDLE game_handle;
    std::string executable_version;

    Version game_version;
    Flag level_flags;

    // instance variables to for timer start-stop logic
    int current_value;

    clock_t timer;
    Split split;

    int addon_time;
    int reset_value;
    bool save_time;
    bool timer_on;

public:
    Timer();

    std::string update_data();

    ~Timer();
};

static HANDLE get_handle(const LPCSTR & lpWindowName, const LPCSTR & lpClassName);
static std::string get_executable_version(const HANDLE & handle);
static uint64_t get_handle_address(const HANDLE & handle);
static HMODULE get_dll_hmodule(const HANDLE & handle, const std::wstring & moduleName);
static std::string read_string_from_memory(const HANDLE & handle, const uint64_t & address, const int & length);
static int read_int_byte_from_memory(const HANDLE & handle, const uint64_t & address);
static float read_float_from_memory(const HANDLE & handle, const uint64_t & address);
static unsigned long read_ulong_from_memory(const HANDLE & handle, const uint64_t & address);
static unsigned int read_uint_from_memory(const HANDLE & handle, const uint64_t & address);
static std::string convert_milliseconds(const int & clock_ticks);
