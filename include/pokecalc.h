#pragma once

struct app_offscreen_buffer {
    void *Memory;
    int Width;
    int Height;
    int Pitch;
};

struct color_gradient_info {
    int ColorBase;
    int ColorSteps;

    uint8_t *Blue;
    uint8_t *Green;
};

internal void AppUpdateAndRender(
    app_offscreen_buffer *Buffer,
    color_gradient_info *ColorGradientInfo,
    int BlueOffset,
    int GreenOffset);
