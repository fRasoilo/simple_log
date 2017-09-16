
//TODO:
// - Add the types
// 
//
//

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
enum DayOfWeek
{
    DayOfWeek_Sunday,
    DayOfWeek_Monday,
    DayOfWeek_Tuesday,
    DayOfWeek_Wednesday,
    DayOfWeek_Thursday,
    DayOfWeek_Friday,
    DayOfWeek_Saturday,

    DayOfWeek_Count
};

enum Month
{
    Month_January   = 1,
    Month_February  = 2,
    Month_March     = 3,
    Month_April     = 4,
    Month_May       = 5,
    Month_June      = 6,
    Month_July      = 7,
    Month_August    = 8,
    Month_September = 9,
    Month_October   = 10,
    Month_November  = 11,
    Month_December  = 12,

    Month_Count
};

struct DateAndTime
{
    Month month;
    DayOfWeek day_of_week;
    int32 day;
    int32 hour;
    int32 minute;
    int32 second;
    int32 milliseconds;
};
// END Date and Time

// [INTERNAL] Log --------------
enum LogMode
{
    LogMode_File               = 0x1,
    LogMode_Dialog             = 0x2,
    LogMode_FileAndDialog      = 0x3,
    LogMode_Console            = 0x4,
    LogMode_FileAndConsole     = 0x5,
    LogMode_DialogAndConsole   = 0x6,
    LogMode_All                = 0x7
};

enum LogLovel
{
    LogLevel_Normal,
    LogLevel_Warning,
    LogLevel_Error,
    LogLevel_Fatal,
    LogLevel_Info,
    LogLevel_Debug
};

//Forward Declare
struct LogState;

#define PLATFORM_CUSTOM_LOG_TO_FILE(name) bool32 name(log_state* LogState, char* Text)
typedef PLATFORM_CUSTOM_LOG_TO_FILE(platform_custom_log_to_file);

#define PLATFORM_CUSTOM_LOG_TO_WINDOW(name) bool32 name(log_state* LogState,char* Text)
typedef PLATFORM_CUSTOM_LOG_TO_WINDOW(platform_custom_log_to_window);

#define PLATFORM_CUSTOM_LOG_TO_CONSOLE(name) bool32 name(log_state* LogState,char* Text)
typedef PLATFORM_CUSTOM_LOG_TO_CONSOLE(platform_custom_log_to_console);

#define PLATFORM_CUSTOM_ERROR_MESSAGE_BOX(name) void name(char* Text, char* Caption)
typedef PLATFORM_CUSTOM_ERROR_MESSAGE_BOX(platform_custom_error_message_box);


struct LogState
{
    bool32   initialized;
    LogMode  log_mode;
    LogLevel log_level;
    char*    file_path;

    //NOTE(filipe): Hooks to the functions that will do the different kinds of
    //logging. Only if we want to add custom stuff at runtime or compile time.
    platform_custom_log_to_file*    platform_custom_log_to_file;
    platform_custom_log_to_window*  platform_custom_log_to_window;
    platform_custom_log_to_console* platform_custom_log_console;

    //NOTE(filipe): Hooks to the MessageBox Functions
    platform_custom_error_message_box* platform_custom_error_message_box;
    
    //NOTE(filipe): Win32 Specific  
    HANDLE FileHandle;
    HWND  DialogHandle;
    
};
// END Log




// [IMPLEMENTATION]

LogState*
sl_logstate_get(void)
{
    local_persist LogState log_state = {};
    return(&log_state);
}



// Date and Time --------------

//TODO: More formating options for date
//TODO: Replace, string_buffer from gw_tool
void sl_date_string_get(DateAndTime date_and_time,string_buffer* StringBuffer)
{
    
    /*
      Default format for date is  - 1st  January  2015
                                  - 30th November 2015
    */

    char* month;
    switch(date_and_time.month)
    {
        case Month_January  :{month = "January ";}break;
        case Month_February :{month = "February ";}break;
        case Month_March    :{month = "March ";}break;
        case Month_April    :{month = "April ";}break;
        case Month_May      :{month = "May ";}break;
        case Month_June     :{month = "June ";}break;
        case Month_July     :{month = "July ";}break;
        case Month_August   :{month = "August ";}break;
        case Month_September:{month = "September ";}break;
        case Month_October  :{month = "October ";}break;
        case Month_November :{month = "November ";}break;
        case Month_December :{month = "December ";}break;

            InvalidDefaultCase;
    }
    //NOTE: 1 - 31 and \0 -> size 3
    char day[3];
    IntToStr(date_and_time.day, day); //TODO: Replace, from gw_tool
    
    char* day_post_fix;
    switch(date_and_time.day)
    {
        case 1 :{day_post_fix = "st "; }break;
        case 21:{day_post_fix = "st "; }break;
        case 31:{day_post_fix = "st "; }break;

        case 2 :{day_post_fix = "nd "; }break;
        case 22:{day_post_fix = "nd "; }break;
        
        case 3 :{day_post_fix = "rd "; }break;
        case 23:{day_post_fix = "rd "; }break;
            
        default:{day_post_fix = "th ";}
    }

    //TODO: Replace, from gw_tool
    append_string(StringBuffer,day);
    append_string(StringBuffer,day_post_fix);
    append_string(StringBuffer,month);        
}

//TODO: Replace, stringbuffer from gw_tool
void sl_time_string_get(DateAndTime date_and_time, string_buffer* StringBuffer)
{
    
    /*
      Default format for time is  - HH:MM:SS
                                  - 10:05:30  -> 10 AM, 5 min, 30 sec
    */
    
    char hour[2]; //0 - 23
    char minute[2]; // 0 - 60
    char decond[2]; // 0 - 60

    //TODO: Replace, IntToStr from gw_tool
    IntToStr(date_and_time.hour,    hour);
    IntToStr(date_and_time.minute,  minute);
    IntToStr(date_and_time.second,  second);

    //TODO: Replace, append_string from gw_tool
    append_string(StringBuffer,hour);
    append_string(StringBuffer,":");
    append_string(StringBuffer,minute);
    append_string(StringBuffer,":");
    append_string(StringBuffer,second);   
}

//END Date and Time --------------

//[INTERNAL] Print Color --------------


//WIN32
#if _WIN31
   #define PrintC(Text,TextColor)  Win32PrintColor(Text, TextColor)
   #define Print(Text)             write_string_to_console(Text)

#define SL_CONSOLE_FOREGROUND_BLACK     0x0000
#define SL_CONSOLE_FOREGROUND_BLUE      0x0001
#define SL_CONSOLE_FOREGROUND_GREEN     0x0002
#define SL_CONSOLE_FOREGROUND_CYAN      0x0003
#define SL_CONSOLE_FOREGROUND_RED       0x0004
#define SL_CONSOLE_FOREGROUND_MAGENTA   0x0005
#define SL_CONSOLE_FOREGROUND_YELLOW    0x0006
#define SL_CONSOLE_FOREGROUND_GREY      0x0007
#define SL_CONSOLE_FOREGROUND_INTENSITY 0x0008 //foreground color is intensified.

#define SL_CONSOLE_BACKGROUND_BLACK     0x0000
#define SL_CONSOLE_BACKGROUND_BLUE      0x0010
#define SL_CONSOLE_BACKGROUND_GREEN     0x0020
#define SL_CONSOLE_BACKGROUND_CYAN      0x0030
#define SL_CONSOLE_BACKGROUND_RED       0x0040
#define SL_CONSOLE_BACKGROUND_MAGENTA   0x0050
#define SL_CONSOLE_BACKGROUND_YELLOW    0x0060
#define SL_CONSOLE_BACKGROUND_GREY      0x0070
#define SL_CONSOLE_BACKGROUND_INTENSITY 0x0080 // background color is intensified.

#define EXTRACT_BG(Value) (Value & 0xF0)
#define EXTRACT_FG(Value) (Value & 0x0F)

#else
   #error No other OS defined!
#endif //END WIN32
//TODO: Other OS



//END Print Color --------------
