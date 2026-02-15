#pragma once

struct linux_offscreen_buffer {
    void *Memory;
    int Size;
    int Width;
    int Height;
    int Pitch;
};

struct linux_color_gradient_info {
    int ColorBase;
    int ColorSteps;

    uint8_t Blue[GlobalColorSteps];
    uint8_t Green[GlobalColorSteps];
};
