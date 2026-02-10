// NOTE: (marco) App code here
//
#include "pokecalc.h"

internal void RenderWeirdGradient(
    app_offscreen_buffer *Buffer,
    color_gradient_info *ColorGradientInfo,
    int BlueOffset,
    int GreenOffset) {

    uint8_t *Row = (uint8_t *)Buffer->Memory;
    for (int Y = 0; Y < Buffer->Height; ++Y) {
        uint32_t *Cell = (uint32_t *)Row;

        for (int X = 0; X < Buffer->Width; ++X) {
            int StepX = (X + BlueOffset) % ColorGradientInfo->ColorSteps;
            int StepY = (Y + GreenOffset) % ColorGradientInfo->ColorSteps;
            int Step = (StepX + StepY) % ColorGradientInfo->ColorSteps;

            uint8_t Glyph = ' ';
            uint8_t ColorPairIndex = ColorGradientInfo->ColorBase + Step;

            *Cell++ = ((ColorPairIndex << 8) | Glyph);
        }

        Row += Buffer->Pitch;
    }
}

internal void UpdateGradient(
    color_gradient_info *ColorGradientInfo,
    int BlueOffset,
    int GreenOffset) {

    for (int i = 0; i < ColorGradientInfo->ColorSteps; i++) {
        int x = i + BlueOffset;
        int y = i + GreenOffset;

        ColorGradientInfo->Blue[i] = (uint8_t)(x % ColorGradientInfo->ColorSteps);
        ColorGradientInfo->Green[i] = (uint8_t)(y % ColorGradientInfo->ColorSteps);
    }
}

internal void AppUpdateAndRender(
    app_memory *Memory,
    app_keyboard_input *Input,
    app_offscreen_buffer *Buffer,
    color_gradient_info *ColorGradientInfo) {

    app_state *AppState = (app_state *)Memory->PermanentStorage;
    if (!Memory->IsInitialized) {
        AppState->BlueOffset = 0;
        AppState->GreenOffset = 0;
    }

    if (Input->Up.EndedDown) {
        AppState->GreenOffset += 1;
    }
    if (Input->Left.EndedDown) {
        AppState->BlueOffset += 1;
    }

    RenderWeirdGradient(
        Buffer,
        ColorGradientInfo,
        AppState->BlueOffset,
        AppState->GreenOffset);

    UpdateGradient(
        ColorGradientInfo,
        AppState->BlueOffset,
        AppState->GreenOffset);
}
