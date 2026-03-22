#pragma once

/*
    NOTE:(marco):

    POKECALC_INTERNAL:
        0 - Build for public release
        1 - Build for developer only

    POKECALC_SLOW:
        0 - No slow code allowed!
        1 - Slow code welcome
 */

#define _LARGEFILE64_SOURCE

#include <stdint.h>
#include <unistd.h>

#define internal static
#define local_persist static
#define global_variable static

global_variable int GlobalColorBase = 16;
global_variable const int GlobalColorSteps = 64;

typedef off_t off64;

#if POKECALC_SLOW
#define Assert(Expression) \
    if (!(Expression)) {   \
        *(int *)0 = 0;     \
    }
#else
#define Assert(Expression)
#endif

// TODO: (marco): Should these always be 64 bit?
#define Kilobytes(Value) ((Value) * 1024LL)
#define Megabytes(Value) (Kilobytes(Value) * 1024LL)
#define Gigabytes(Value) (Megabytes(Value) * 1024LL)
#define Terabytes(Value) (Gigabytes(Value) * 1024LL)

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))
// TODO: (marco) macros: swap, min, max ...

/*
    NOTE:(marco): Services that the platform provides to the game
*/
#if POKECALC_INTERNAL
/* WARNING:(marco):

   These are not for doing anything in the shipping app - they are
   blocking and the write doen't protect against lost data!
 */
struct debug_read_file_result {
    int64_t ContentsSize;
    void *Contents;
};

#define DEBUG_PLATFORM_FREE_FILE_MEMORY(name) void name(void *Memory, int64_t MemorySize)
typedef DEBUG_PLATFORM_FREE_FILE_MEMORY(debug_platform_free_file_memory);

#define DEBUG_PLATFORM_READ_ENTIRE_FILE(name) debug_read_file_result name(const char *Filename)
typedef DEBUG_PLATFORM_READ_ENTIRE_FILE(debug_platform_read_entire_file);

#define DEBUG_PLATFORM_WRITE_ENTIRE_FILE(name) bool name(const char *Filename, int64_t MemorySize, void *Memory)
typedef DEBUG_PLATFORM_WRITE_ENTIRE_FILE(debug_platform_write_entire_file);

#endif

struct app_offscreen_buffer {
    void *Memory;
    int Width;
    int Height;
    int Pitch;
};

struct color_gradient_info {
    int ColorBase;
    int ColorSteps;

    uint8_t Blue[GlobalColorSteps];
    uint8_t Green[GlobalColorSteps];
};

struct app_key_state {
    int HalfTransitionCount;
    bool EndedDown;
};

struct app_keyboard_input {
    union {
        app_key_state Keys[7];

        struct {
            app_key_state Up;
            app_key_state Down;
            app_key_state Left;
            app_key_state Right;
            app_key_state Select;
            app_key_state Back;
            app_key_state Help;
        };
    };
};

struct app_memory {
    bool IsInitialized;
    uint64_t PermanentStorageSize;
    void *PermanentStorage;

    uint64_t TransientStorageSize;
    void *TransientStorage;

    debug_platform_free_file_memory *DEBUGPlatformFreeFileMemory;
    debug_platform_read_entire_file *DEBUGPlatformReadEntireFile;
    debug_platform_write_entire_file *DEBUGPlatformWriteEntireFile;
};

#define APP_UPDATE_AND_RENDER(name) void name(app_memory *AppMemory, app_keyboard_input *Input, app_offscreen_buffer *Buffer, color_gradient_info *ColorGradientInfo)
typedef APP_UPDATE_AND_RENDER(app_update_and_render);
APP_UPDATE_AND_RENDER(AppUpdateAndRenderStub) {
}

//
//
//
struct app_state {
    int BlueOffset;
    int GreenOffset;
};
