
/*
  -Changes from internal lib to simple_log.h

    ->

 */
  
//TODO:
// - Add the types
// 
//
//

#pragma once

//Simple Log --------------

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

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

// [INTERNAL] Types --------------
#include <stdint.h>
//Unsigned
typedef uint32_t uint32;
//Signed
typedef int32_t int32;
//Bool
typedef int32 bool32;


// [INTERNAL] Static Declarations
#define internal        static
#define global_variable static
#define local_persist   static

// [INTERNAL] Utility Macros
#define Bytes(n)  (n)
#define KiloBytes(n)  (Bytes(n)*1024)
#define InvalidCodePath Assert(!"InvalidCodePath")
#define InvalidDefaultCase default: {InvalidCodePath;} break
#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}

// [INTERNAL] Log Buffer
#define LOG_BUFFER_SIZE KiloBytes(1)
struct LogBuffer
{
    char buffer[LOG_BUFFER_SIZE];
    uint32 used;
};

global_variable LogBuffer log_buffer;
//


// [INTERNAL]  Date and Time 
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

enum LogLevel
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
internal bool32 sl_buffer_append_string(LogBuffer* log_buffer, char* string);
inline  int32 sl_string_length(char* string);
inline int32 sl_string_size(char* string);

#define PLATFORM_CUSTOM_LOG_TO_FILE(name) bool32 name(LogState* log_state, char* text)
typedef PLATFORM_CUSTOM_LOG_TO_FILE(platform_custom_log_to_file);

#define PLATFORM_CUSTOM_LOG_TO_WINDOW(name) bool32 name(LogState* log_state,char* text)
typedef PLATFORM_CUSTOM_LOG_TO_WINDOW(platform_custom_log_to_window);

#define PLATFORM_CUSTOM_LOG_TO_CONSOLE(name) bool32 name(LogState* log_state,char* text)
typedef PLATFORM_CUSTOM_LOG_TO_CONSOLE(platform_custom_log_to_console);

#define PLATFORM_CUSTOM_ERROR_MESSAGE_BOX(name) void name(char* text, char* caption)
typedef PLATFORM_CUSTOM_ERROR_MESSAGE_BOX(platform_custom_error_message_box);


struct LogState
{
    bool32   initialized;
    LogMode  log_mode;
    LogLevel log_level;
    char*    file_path;

    //NOTE(filipe): Hooks to the functions that will do the different kinds of
    //logging. Only if we want to add custom stuff at runtime or compile time.
    
    platform_custom_log_to_file*    log_to_file_func;
    platform_custom_log_to_window*  log_to_window_func;
    platform_custom_log_to_console* log_to_console_func;
    //NOTE(filipe): Hooks to the MessageBox Functions
    platform_custom_error_message_box* error_message_box_func;
    
    //NOTE(filipe): Win32 Specific  
    HANDLE FileHandle;
    HWND   DialogHandle;
    
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
void sl_date_string_get(DateAndTime date_and_time,LogBuffer* buffer)
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
    
    //IntToStr(date_and_time.day, day); //TODO: Check!
    sprintf(day, "%d",date_and_time.day);
    
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

    sl_buffer_append_string(buffer, day);
    sl_buffer_append_string(buffer, day_post_fix);
    sl_buffer_append_string(buffer, month);        
    
}

//TODO: Replace, stringbuffer from gw_tool
void sl_time_string_get(DateAndTime date_and_time, LogBuffer* buffer)
{
    
    /*
      Default format for time is  - HH:MM:SS
                                  - 10:05:30  -> 10 AM, 5 min, 30 sec
    */
    
    char hour[2]; //0 - 23
    char minute[2]; // 0 - 60
    char second[2]; // 0 - 60

    //TODO: Check
    sprintf(hour, "%d",date_and_time.hour);
    sprintf(minute, "%d",date_and_time.minute);
    sprintf(second, "%d",date_and_time.second);
    //IntToStr(date_and_time.hour,    hour);
    //IntToStr(date_and_time.minute,  minute);
    //IntToStr(date_and_time.second,  second);

    sl_buffer_append_string(buffer,hour);
    sl_buffer_append_string(buffer, ":");
    sl_buffer_append_string(buffer, minute);
    sl_buffer_append_string(buffer, ":");
    sl_buffer_append_string(buffer, second);
    
}

//END Date and Time --------------

//[INTERNAL] Print Color --------------


//WIN32
#if _WIN32
   #define sl_print_color(Text,TextColor)  sl_win32_print_color(Text, TextColor)
   #define sl_print(Text)                  sl_win32_string_write_to_console(Text)

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

//Win32 Specific --------------

internal bool32 sl_win32_string_write_to_console(char* string);


//NOTE: Does not support Black BG unless the console is already black.
void sl_win32_print_color(char* text, WORD ColorAttributes)
{
    HANDLE StdOut;
    CONSOLE_SCREEN_BUFFER_INFO ConsoleScreenBufferInfo;

    //TODO:Maybe just attach in one central point when we
    //choose to log to the console? Instead of calling AttachConsole over and over
    // again here.
    AttachConsole(ATTACH_PARENT_PROCESS);
    StdOut = GetStdHandle(STD_OUTPUT_HANDLE); //STD_OUTPUT_HANDLE (DWORD)-11  (char)-11

    if((StdOut) && (StdOut != INVALID_HANDLE_VALUE))
    {
        GetConsoleScreenBufferInfo(StdOut, &ConsoleScreenBufferInfo);
        WORD OldColorAttribs = ConsoleScreenBufferInfo.wAttributes;
        WORD CurrentBG = EXTRACT_BG(OldColorAttribs);
        WORD ColorAttributesBG = EXTRACT_BG(ColorAttributes);
        if(ColorAttributesBG)
        {
            CurrentBG = ColorAttributesBG;
        }
        SetConsoleTextAttribute(StdOut,ColorAttributes|CurrentBG);

        sl_win32_string_write_to_console(text);
        SetConsoleTextAttribute(StdOut, OldColorAttribs);
    }    
}


DateAndTime sl_win32_date_and_time_get()
{
    DateAndTime date_and_time = {};
    SYSTEMTIME system_time = {};

    GetLocalTime(&system_time);

    date_and_time.month        = (Month)system_time.wMonth;
    date_and_time.day_of_week  = (DayOfWeek)system_time.wDayOfWeek;
    date_and_time.day          = system_time.wDay;
    date_and_time.hour         = system_time.wHour;
    date_and_time.minute       = system_time.wMinute;
    date_and_time.second       = system_time.wSecond;
    date_and_time.milliseconds = system_time.wMilliseconds;
    return(date_and_time);
}


internal bool32 sl_win32_default_log_to_list_box(LogState* log_state, char* text)
{
    bool32 result = false;
    
    //NOTE: From SongHo -> "SendMessage() in worker thread may cause deadlock
    //                              , use SendMessageTimeOut() instead"
    int32 list_index;
    
    /*
      IMPORTANT
      NOTE:We must make sure to call Win32SetLogWindow to initialize the
      window that will be used for logging. This can be done before or after
      InitLog is called. As matter of convention it is usually called after.

      Before any logging can take place in Win32 we must call InitLog and
      Win32SetLogWindow at least once!
    */

    //TODO: Replace stringbuffer and append_string from gw_tool
    if(log_state->DialogHandle)
    {
        LogBuffer buffer_to_write_out = {};
        sl_buffer_append_string(&buffer_to_write_out,"[");

        sl_date_string_get(sl_win32_date_and_time_get(),&buffer_to_write_out);
        sl_time_string_get(sl_win32_date_and_time_get(),&buffer_to_write_out);

        
        sl_buffer_append_string(&buffer_to_write_out,"] ");
        sl_buffer_append_string(&buffer_to_write_out,text);

#define IDC_LIST_LOG 12
        HWND ListHandle = GetDlgItem(log_state->DialogHandle, IDC_LIST_LOG);
        result = SendMessageTimeout(ListHandle,
                                    LB_ADDSTRING,
                                    0,
                                    (LPARAM)buffer_to_write_out.buffer,
                                    SMTO_NORMAL | SMTO_ABORTIFHUNG,
                                    500,(PDWORD_PTR)&list_index);
        if(result)
        {
            SendMessageTimeout(ListHandle,
                               LB_SETTOPINDEX,
                               list_index,
                               0,
                               SMTO_NORMAL | SMTO_ABORTIFHUNG,
                               500, 0);
        }
    }    
    return(result);
}



internal bool32
sl_win32_default_log_to_file(LogState* log_state, char* text)
{
    bool32 result = false;

    LogBuffer buffer_to_write_out = {};
    sl_buffer_append_string(&buffer_to_write_out,"[");    
    sl_date_string_get(sl_win32_date_and_time_get(),&buffer_to_write_out);
    sl_time_string_get(sl_win32_date_and_time_get(),&buffer_to_write_out);

    
    sl_buffer_append_string(&buffer_to_write_out,"] ");
    sl_buffer_append_string(&buffer_to_write_out,text );
    
    if(log_state->FileHandle)
    {
        DWORD bytes_written = 0;
        //TODO: Replace StringByteSize from gw_tool
        result = WriteFile(log_state->FileHandle,
                           buffer_to_write_out.buffer,
                           sl_string_size(buffer_to_write_out.buffer),
                           &bytes_written, 0);
    }
    else
    {
        log_state->FileHandle = CreateFile(log_state->file_path,
                                          GENERIC_WRITE,
                                          FILE_SHARE_READ,
                                          0,
                                          CREATE_ALWAYS, //NOTE: Will always overwrite the file that was there before.
                                          FILE_ATTRIBUTE_NORMAL, 
                                          0);
        if(log_state->FileHandle != INVALID_HANDLE_VALUE)
        {
            DWORD bytes_written = 0;
            //TODO: Replace StringByteSize from gw_tool
            result = WriteFile(log_state->FileHandle,
                               buffer_to_write_out.buffer,
                               sl_string_size(buffer_to_write_out.buffer),
                               &bytes_written, 0);
        }
    }
    return(result);
}


internal void
sl_win32_default_log_to_console(LogState* log_state, char* text)
{
    switch(log_state->log_level)
    {
        case LogLevel_Normal : { sl_print(text); }break;
        case LogLevel_Warning: { sl_print_color(text, SL_CONSOLE_FOREGROUND_YELLOW|SL_CONSOLE_FOREGROUND_INTENSITY|SL_CONSOLE_BACKGROUND_RED); }break;
        case LogLevel_Error  : { sl_print_color(text, SL_CONSOLE_FOREGROUND_MAGENTA|SL_CONSOLE_FOREGROUND_INTENSITY|SL_CONSOLE_BACKGROUND_GREY); }break;
        case LogLevel_Fatal  : { sl_print_color(text, SL_CONSOLE_FOREGROUND_RED|SL_CONSOLE_FOREGROUND_INTENSITY|SL_CONSOLE_BACKGROUND_GREY); }break;
        case LogLevel_Info   : { sl_print_color(text, SL_CONSOLE_FOREGROUND_GREEN|SL_CONSOLE_FOREGROUND_INTENSITY|SL_CONSOLE_BACKGROUND_BLACK); }break;
        case LogLevel_Debug  : { sl_print_color(text,SL_CONSOLE_FOREGROUND_CYAN|SL_CONSOLE_FOREGROUND_INTENSITY|SL_CONSOLE_BACKGROUND_BLACK); }break;
        InvalidDefaultCase;                
    }
}


internal void sl_win32_default_error_message_box(char* text, char* caption = "Error!")
{
    MessageBox(0,text,caption,MB_OK|MB_ICONERROR);
}


bool32 sl_win32_log_window_set(HWND dialog)
{
    bool32 result = false;
    LogState* log_state = sl_logstate_get();
    log_state->DialogHandle = dialog;
    if(log_state->DialogHandle)
    {
        result = true;
    }
    return(result);
}

//END Win32 Specific --------------

//Platform Independent --------------

//TODO: Need to make some different versions of this function, otherwise
//we are not able no init with only a few of the custom hooks. For example in this
//version we cant init custom hook for log to window without also initing file and
//console.

void sl_log_init(LogMode log_mode, char* file_path = '\0',
                 platform_custom_log_to_file*       platform_custom_log_to_file = 0,
                 platform_custom_log_to_console*    platform_custom_log_to_console = 0,
                 platform_custom_log_to_window*     platform_custom_log_to_window = 0,
                 platform_custom_error_message_box* platform_custom_error_message_box = 0)
{
    LogState* log_state = sl_logstate_get();

    //TODO: Replace Assert, from gw_tool
    Assert(log_state);
    log_state->initialized = true;
    log_state->log_mode  = (LogMode)log_mode;
    log_state->file_path = file_path;


    //Set to defualt first and then override if we passed in any custom functions.
#if _Win32
    LogState->PlatformCustomLogToFile = &Win32LogToFileDEFAULT;
    LogState->PlatformCustomLogToWindow = &Win32LogToListBoxDEFAULT;
    LogState->PlatformCustomLogToConsole = &Win32LogToConsoleDEFAULT;
    LogState->PlatformCustomErrorMessageBox = &Win32ErrorMessageBoxDEFAULT;        
#else
    //TODO: Add defaults for other OS's when needed!
    Assert(!"No default log functions set for this OS! Crash and burn!");
#endif // _Win32

    
    //Hook up to the custom log functions or fallback to the defaults.
    if(platform_custom_log_to_file){
        log_state->log_to_file_func = platform_custom_log_to_file;
    }
    if(platform_custom_log_to_console){
        log_state->log_to_console_func = platform_custom_log_to_console;
    }
    if(platform_custom_log_to_window){
        log_state->log_to_window_func = platform_custom_log_to_window;
    }
    if(platform_custom_error_message_box){
        log_state->error_message_box_func = platform_custom_error_message_box;
    }
    
    Assert(log_state->log_to_file_func);
    Assert(log_state->log_to_console_func);
    Assert(log_state->log_to_window_func);
    Assert(log_state->error_message_box_func);
}

//TODO: Replace internal, from gw_tool
internal void
sl_log_level_change(LogLevel log_level)
{
    LogState* log_state = sl_logstate_get();
    log_state->log_level = log_level;
}

internal void
sl_log_level_reset()
{
    LogState* log_state = sl_logstate_get();
    log_state->log_level = LogLevel_Normal;
}


internal bool32
sl_log(char* text)
{
    bool32 result = false;
   
    //TODO: Also at the moment we are assuming the Log Window is already
    //created and shown. If its not? Need to buffer up messages and show them when
    //log window is presented? (To worry about in the future). For now lets just skip
    //if there is still no window
        
    LogState* log_state = sl_logstate_get();
    Assert(log_state);
    
    if(log_state->initialized)
    {
        switch(log_state->log_mode)
        {
            case LogMode_File         : { result = log_state->log_to_file_func(log_state,text); }break;
            case LogMode_Dialog       : { result = log_state->log_to_window_func(log_state,text); }break;
            case LogMode_Console      : { result = log_state->log_to_console_func(log_state,text); }break;
                
            case LogMode_FileAndDialog:
            {
                result = log_state->log_to_file_func(log_state,text);
                result &= log_state->log_to_window_func(log_state,text);                
            }break;
            
            case LogMode_FileAndConsole:
            {
                result  = log_state->log_to_file_func(log_state,text);
                result &= log_state->log_to_console_func(log_state,text);                
            }break;

            case LogMode_DialogAndConsole:
            {
                result =  log_state->log_to_window_func(log_state,text);
                result &= log_state->log_to_console_func(log_state,text);                
            }break;

            case LogMode_All:
            {
                result =  log_state->log_to_file_func(log_state,text);
                result &= log_state->log_to_window_func(log_state,text);
                result &= log_state->log_to_console_func(log_state,text);                
            }break;

            InvalidDefaultCase;
        }
    }
    else
    {
        Assert(!"LogState is not initialized");
    }
    return(result);
}


internal bool32
sl_logf(char* fmt, ...)
{

    //TODO: Replace this with the std lib function

    //IMPORTANT NOTE(filipe): Win32 Specific?

	Assert(ASM_LOADED);
    x64SpillRegisters_R();    
    void *OptionalArgument = (char*)(&Fmt) + 8;
    //
    
    string_buffer Buffer ={};
    print_to_buffer_(&Buffer, Fmt, OptionalArgument);
    char* Text = Buffer.Buffer;
    
    bool32 Result = false;
   
    //TODO: Also at the moment we are assuming the Log Window is already
    //created and shown. If its not? Need to buffer up messages and show them when
    //log window is presented? x(To worry about in the future). For now lets just skip
    //if there is still no window
        
    LogState* log_state = sl_logstate_get();
    Assert(log_state);
    
    if(log_state->initialized)
    {
        switch(log_state->log_mode)
        {
            case LogMode_File:
            {
                Result = log_state->PlatformCustomLogToFile(log_state,Text);
            }break;

            case LogMode_dialog:
            {
                Result = log_state->PlatformCustomLogToWindow(log_state,Text);
            }break;
            
            case LogMode_console:
            {
                Result = log_state->PlatformCustomLogToConsole(log_state,Text);
            }break;

            case LogMode_file_and_dialog:
            {
                Result = log_state->PlatformCustomLogToFile(log_state,Text);
                Result &= log_state->PlatformCustomLogToWindow(log_state,Text);                
            }break;

            case LogMode_file_and_console:
            {
                Result  = log_state->PlatformCustomLogToFile(log_state,Text);
                Result &= log_state->PlatformCustomLogToConsole(log_state,Text);                
            }break;

            case LogMode_dialog_and_console:
            {
                Result = log_state->PlatformCustomLogToWindow(log_state,Text);
                Result &= log_state->PlatformCustomLogToConsole(log_state,Text);                
            }break;

            case LogMode_all:
            {
                Result = log_state->PlatformCustomLogToFile(log_state,Text);
                Result &= log_state->PlatformCustomLogToWindow(log_state,Text);
                Result &= log_state->PlatformCustomLogToConsole(log_state,Text);                
            }break;

            InvalidDefaultCase;
        }
    }
    else
    {
        Assert(!"LogState is not initialized");
    }
    return(Result);
}


internal bool32
sl_log_error(char* text)
{
    //TODO: Replace string_buffer from gw_tool
    string_buffer BufferToWriteOut = {};
    append_string(&BufferToWriteOut,"[ERROR]: " );
    append_string(&BufferToWriteOut,Text );

    sl_log_level_change(LogLevel_Error);
    bool32 result = sl_log(BufferToWriteOut.Buffer);
    sl_log_level_reset();
    return(result);    
}

/*
All The 'F' variants go here
 */


internal bool32
sl_log_errorf(char* fmt, ...)
{
 
    //IMPORTANT NOTE(filipe): Win32 Specific?
    Assert(ASM_LOADED);
    x64SpillRegisters_R();    
    void *OptionalArgument = (char*)(&Fmt) + 8;
    //

    string_buffer BufferToWriteOut = {};
    append_string(&BufferToWriteOut,"[ERROR]: " );
    print_to_buffer_(&BufferToWriteOut, Fmt, OptionalArgument);

    sl_log_level_change(LogLevel_Error);
    bool32 result = sl_log(BufferToWriteOut.Buffer);
    sl_log_level_reset();
    return(result);    
}


internal bool32
sl_log_warning(char* text)
{
    string_buffer BufferToWriteOut = {};
    append_string(&BufferToWriteOut,"[WARNING]: ");
    append_string(&BufferToWriteOut,Text);

    sl_log_level_change(LogLevel_Warning);
    bool32 result = sl_log(BufferToWriteOut.Buffer);
    sl_log_level_reset();
    return(result);    
}


internal bool32
sl_log_warningf(char* fmt, ...)
{
    //IMPORTANT NOTE(filipe): Win32 Specific?
    Assert(ASM_LOADED);
    x64SpillRegisters_R();    
    void *OptionalArgument = (char*)(&Fmt) + 8;
    //

    string_buffer BufferToWriteOut = {};
    append_string(&BufferToWriteOut,"[WARNING]: ");
    print_to_buffer_(&BufferToWriteOut, Fmt, OptionalArgument);

    sl_log_level_change(LogLevel_Warning);
    bool32 result = Log(BufferToWriteOut.Buffer);
    ResetLogLevel();
    return(Result);    
}


internal bool32
sl_log_info(char* text)
{
    string_buffer BufferToWriteOut = {};
    append_string(&BufferToWriteOut,"[INFO]: ");
    append_string(&BufferToWriteOut,Text);
    
    sl_log_level_change(LogLevel_Info);
    bool32 result = sl_log(BufferToWriteOut.Buffer);
    sl_log_level_reset();
    return(result);    
}

internal bool32
sl_log_infof(char* fmt, ...)
{
    //IMPORTANT NOTE(filipe): Win32 Specific?
    Assert(ASM_LOADED);
    x64SpillRegisters_R();    
    void *OptionalArgument = (char*)(&Fmt) + 8;
    //

    string_buffer BufferToWriteOut = {};
    append_string(&BufferToWriteOut,"[INFO]: ");
    print_to_buffer_(&BufferToWriteOut, Fmt, OptionalArgument);

    sl_log_level_change(LogLevel_Info);
    bool32 result = sl_log(BufferToWriteOut.Buffer);
    sl_log_level_reset();
    return(result);    
}


internal bool32
sl_log_debug(char* text)
{
    string_buffer BufferToWriteOut = {};
    append_string(&BufferToWriteOut,"[DEBUG]: " );
    append_string(&BufferToWriteOut,Text);

    sl_log_level_change(LogLevel_Debug);
    bool32 result = sl_log(BufferToWriteOut.Buffer);
    sl_log_level_reset();
    return(result);    
}

internal bool32
sl_log_debugf(char* fmt, ...)
{
    //IMPORTANT NOTE(filipe): Win32 Specific?
    Assert(ASM_LOADED);
    x64SpillRegisters_R();    
    void *OptionalArgument = (char*)(&Fmt) + 8;
    //

    string_buffer BufferToWriteOut = {};
    append_string(&BufferToWriteOut,"[DEBUG]: " );
    print_to_buffer_(&BufferToWriteOut, Fmt, OptionalArgument);

    sl_log_level_change(LogLevel_Debug);
    bool32 Result = sl_log(BufferToWriteOut.Buffer);
    sl_log_level_reset();
    return(result);    
}


internal void
sl_log_fatal(char* text)
{
    //TODO(filipe): Fatal Error should give information about file, line, function
    string_buffer BufferToWriteOut = {};
    append_string(&BufferToWriteOut,"[***FATAL ERROR***]: ");
    append_string(&BufferToWriteOut,Text);

    sl_log_level_change(LogLevel_Fatal);
    sl_log(BufferToWriteOut.Buffer);
    sl_log_level_reset();
    
    //NOTE: Crash on purpose!
    Assert(!"FATAL ERROR");
}

internal void
sl_log_fatalf(char* fmt, ...)
{
    //IMPORTANT NOTE(filipe): Win32 Specific?
    Assert(ASM_LOADED);
    x64SpillRegisters_R();    
    void *OptionalArgument = (char*)(&Fmt) + 8;
    //

    //TODO(filipe): Fatal Error should give information about file, line, function
    string_buffer BufferToWriteOut = {};
    append_string(&BufferToWriteOut,"[***FATAL ERROR***]: ");
    print_to_buffer_(&BufferToWriteOut, Fmt, OptionalArgument);

    sl_log_level_change(LogLevel_Fatal);
    sl_log(BufferToWriteOut.Buffer);
    sl_log_level_reset();
    //NOTE: Crash on purpose!
    Assert(!"FATAL ERROR");
}


internal void
sl_error_message_box(char* text, char* caption = "Error!")
{
    //TODO: Should we log inside here?
    LogState* log_state = sl_logstate_get();
    Assert(log_state);
    if(log_state->initialized)
    {
        log_state->platform_custom_error_message_box(text, caption);
    }
}


internal void
sl_error_message_box_fatal(char* text, char* caption = "FATAL ERROR!")
{
    //TODO: Should we log inside here?
    LogState* log_state = sl_logstate_get();
    Assert(log_state);
    if(log_state->initialized)
    {
        log_state->platform_custom_error_message_box(text, caption);
    }
    //TODO: Replace with debug break!
    Assert(!"FATAL ERROR");
}



//END Platform Independent --------------







//IMPORTANT: NEED TO ORGANIZE THESE BETTER

// Buffer Utilities --------------


inline  int32
sl_string_length(char* string)
{
    int32 count = 0;
    while(*string++)
    {
        ++count;
    }
    return(count);
}


inline int32
sl_string_size(char* string)
{
    int32 length = sl_string_length(string);

    int32 size = length / sizeof(string[0]);
    return(size);
}


internal bool32
sl_buffer_append_string(LogBuffer* log_buffer, char* string)
{
    uint32 string_size = StringByteSize(string);
    //TODO(filipe): In the future, should grow the buffer...
    Assert((StringSize + log_buffer->used) < LOG_BUFFER_SIZE); 
    b32 result = false;

    char* str_ptr = string;
    for(int32 index = log_buffer->used ;*str_ptr; ++index)
    {
        log_buffer->buffer[index] = *str_ptr++;
    }
    log_buffer->used += string_size;

    log_buffer->buffer[log_buffer->used] = '\0';
    
    return(result);
}
//END Buffer Utilites


//TODO: Change name, win32 speci
internal bool32 sl_win32_string_write_to_console(char* string)
{
    bool32 result = false;
    HANDLE StdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if(StdOut != INVALID_HANDLE_VALUE)
    {
        DWORD BytesWritten;
        WriteFile(StdOut,
                  String,
                  StringByteSize(String),
                  &BytesWritten, 0);
        result = true;
    }
    return(Result);
}
