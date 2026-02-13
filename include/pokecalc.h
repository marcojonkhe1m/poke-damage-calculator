#pragma once

#define Kilobytes(Value) ((Value) * 1024)
#define Megabytes(Value) (Kilobytes(Value) * 1024)
#define Gigabytes(Value) (Megabytes(Value) * 1024)

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))
// TODO: (marco) macros: swap, min, max ...

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
