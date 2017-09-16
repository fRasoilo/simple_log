#pragma once

//Simple Logger --------------

//===============================================================================  
//  You MUST define SIMPLE_LOG_IMPLEMENTATION
//
//  in exactly one C or C++ file that includes this header, BEFORE the include
//  like so:
//
//  #define SIMPLE_LOG_IMPLEMENTATION
//  #include "simple_log.h"
//
//  All other files should just include "simple_log.h" without the define
//=============================================================================




// [INTERNAL]  Date and Time --------------
enum day_of_week
{
    DayOfWeek_sunday,
    DayOfWeek_monday,
    DayOfWeek_tuesday,
    DayOfWeek_wednesday,
    DayOfWeek_thursday,
    DayOfWeek_friday,
    DayOfWeek_saturday,

    DayOfWeek_number_of_days
};

enum month
{
    Month_january   = 1,
    Month_february  = 2,
    Month_march     = 3,
    Month_april     = 4,
    Month_may       = 5,
    Month_june      = 6,
    Month_july      = 7,
    Month_august    = 8,
    Month_september = 9,
    Month_october   = 10,
    Month_november  = 11,
    Month_december  = 12,

    Month_number_of_months
};

struct date_and_time
{
    month Month;
    day_of_week DayOfWeek;
    int32 Day;
    int32 Hour;
    int32 Minute;
    int32 Second;
    int32 Milliseconds;
};
// END Date and Time

// [INTERNAL] Log --------------
enum log_mode
{
    LogMode_file               = 0x1,
    LogMode_dialog             = 0x2,
    LogMode_file_and_dialog    = 0x3,
    LogMode_console            = 0x4,
    LogMode_file_and_console   = 0x5,
    LogMode_dialog_and_console = 0x6,
    LogMode_all                = 0x7
};
enum log_level
{
    LogLevel_normal,
    LogLevel_warning,
    LogLevel_error,
    LogLevel_fatal,
    LogLevel_info,
    LogLevel_debug
};

//Forward Declare
struct log_state;

#define PLATFORM_CUSTOM_LOG_TO_FILE(name) bool32 name(log_state* LogState, char* Text)
typedef PLATFORM_CUSTOM_LOG_TO_FILE(platform_custom_log_to_file);

#define PLATFORM_CUSTOM_LOG_TO_WINDOW(name) bool32 name(log_state* LogState,char* Text)
typedef PLATFORM_CUSTOM_LOG_TO_WINDOW(platform_custom_log_to_window);

#define PLATFORM_CUSTOM_LOG_TO_CONSOLE(name) bool32 name(log_state* LogState,char* Text)
typedef PLATFORM_CUSTOM_LOG_TO_CONSOLE(platform_custom_log_to_console);

#define PLATFORM_CUSTOM_ERROR_MESSAGE_BOX(name) void name(char* Text, char* Caption)
typedef PLATFORM_CUSTOM_ERROR_MESSAGE_BOX(platform_custom_error_message_box);


struct log_state
{
    bool32 Initialized;
    log_mode LogMode;
    log_level LogLevel;
    char* FilePath;

    //NOTE(filipe): Hooks to the functions that will do the different kinds of
    //logging. Only if we want to add custom stuff at runtime or compile time.
    platform_custom_log_to_file*   PlatformCustomLogToFile;
    platform_custom_log_to_window* PlatformCustomLogToWindow;
    platform_custom_log_to_console* PlatformCustomLogToConsole;

    //NOTE(filipe): Hooks to the MessageBox Functions
    platform_custom_error_message_box* PlatformCustomErrorMessageBox;
    
    //NOTE(filipe): Win32 Specific  
    HANDLE FileHandle;
    HWND  DialogHandle;
    
};
// END Log




// [IMPLEMENTATION]

log_state*
GetLogState(void)
{
    local_persist log_state LogState = {};
    return(&LogState);
}



