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
    uint64_t ContentsSize;
    void *Contents;
};
internal debug_read_file_result DEBUGPlatformReadEntireFile(const char *Filename);
internal void DEBUGPlatformFreeFileMemory(void *Memory, uint64_t MemorySize);
internal bool DEBUGPlatformWriteEntireFile(const char *Filename, uint64_t MemorySize, void *Memory);
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
};

internal void AppUpdateAndRender(
    app_memory *AppMemory,
    app_keyboard_input *Input,
    app_offscreen_buffer *Buffer,
    color_gradient_info *ColorGradientInfo);

//
//
//
struct app_state {
    int BlueOffset;
    int GreenOffset;
};
