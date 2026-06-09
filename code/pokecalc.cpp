// NOTE: (marco) App code here
//
#include "pokecalc.h"

internal int RoundFloatToInt(float Float) {
    int Result = (int)(Float + 0.5f);
    // TODO: (marco): Intrincic????
    return Result;
}

internal void DrawRectangle(app_offscreen_buffer *Buffer, float RealMinX, float RealMinY, float RealMaxX, float RealMaxY) {
    int MinX = RoundFloatToInt(RealMinX);
    int MinY = RoundFloatToInt(RealMinY);
    int MaxX = RoundFloatToInt(RealMaxX);
    int MaxY = RoundFloatToInt(RealMaxY);

    if (MinX < 0) {
        MinX = 0;
    }

    if (MinY < 0) {
        MinY = 0;
    }

    if (MaxX > Buffer->Width) {
        MaxX = Buffer->Width;
    }

    if (MaxY > Buffer->Height) {
        MaxX = Buffer->Height;
    }

    // TODO: (marco) Add color
    uint32_t Color = 0xFFFFFFFF;

    uint8_t *Row = ((uint8_t *)Buffer->Memory + MinX * Buffer->BytesPerPixel + MinY * Buffer->Pitch);
    for (int y = 0; y < MaxY; ++y) {
        uint32_t *Pixel = (uint32_t *)Row;
        for (int x = 0; x < MaxX; ++x) {
            *Pixel++ = Color;
        }
    }
    Row += Buffer->Pitch;
}

extern "C" APP_UPDATE_AND_RENDER(AppUpdateAndRender) {

    Assert(sizeof(app_state) <= AppMemory->PermanentStorageSize);

    app_state *AppState = (app_state *)AppMemory->PermanentStorage;
    if (!AppMemory->IsInitialized) {

        AppMemory->IsInitialized = true;
    }

    DrawRectangle(Buffer, 10.0f, 10.0f, 30.0f, 30.0f);
}

/*
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

void UpdateGradient(
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
*/
