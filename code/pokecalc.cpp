// NOTE: (marco) App code here
//
#include "pokecalc.h"

internal uint32_t RoundFloatToUInt(float Float) {
    uint32_t Result = (uint32_t)(Float + 0.5f);
    // TODO: (marco): Intrinsic????
    return Result;
}

internal void DrawRectangle(app_offscreen_buffer *Buffer,
    float RealMinX, float RealMinY, float RealMaxX, float RealMaxY,
    float R, float G, float B) {

    int MinX = RoundFloatToUInt(RealMinX);
    int MinY = RoundFloatToUInt(RealMinY);
    int MaxX = RoundFloatToUInt(RealMaxX);
    int MaxY = RoundFloatToUInt(RealMaxY);

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
    uint32_t Color = ((RoundFloatToUInt(R * 255.0f) << 16) | (RoundFloatToUInt(G * 255.0f) << 8) | (RoundFloatToUInt(B * 255.0f) << 8));

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

    DrawRectangle(Buffer, 0.0f, 0.0f, Buffer->Width, Buffer->Height, 1.0f, 0.0f, 1.0f);
    DrawRectangle(Buffer, 10.0f, 10.0f, 40.0f, 40.0, 0.0f, 1.0f, 1.0f);
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
