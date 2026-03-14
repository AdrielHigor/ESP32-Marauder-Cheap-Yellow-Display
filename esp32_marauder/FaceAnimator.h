#pragma once

#include "configs.h"

// Only compiled for display targets (ILI9341 / ST7789 / ST7796)
#if defined(HAS_ILI9341) || defined(HAS_ST7789) || defined(HAS_ST7796)

#include <TFT_eSPI.h>

// ---------------------------------------------------------------------------
// FaceExpression
// ---------------------------------------------------------------------------
enum FaceExpression : uint8_t {
    FACE_IDLE    = 0,   // blue eyes, slow blink — waiting
    FACE_ATTACK  = 1,   // red eyes, squint + frown — scan/attack active
    FACE_SUCCESS = 2,   // arc eyes + big smile — event captured
};

// ---------------------------------------------------------------------------
// FaceAnimator — Eilik-style robot pet, 64×48 px, bottom-right corner
//
// The sprite sits in the hardware-fixed bottom area (BOT_FIXED_AREA=48) so
// the scroll text system never overwrites it.
//
// Usage:
//   FaceAnimator face(display_obj.tft);
//   face.begin();                        // once in setup(), after RunSetup()
//   face.setExpression(FACE_ATTACK);     // call anywhere to change state
//   face.update(millis());               // call every loop() — self-rate-limited
// ---------------------------------------------------------------------------
class FaceAnimator {
public:
    explicit FaceAnimator(TFT_eSPI& tft);

    void begin();
    void setExpression(FaceExpression expr);
    void update(uint32_t nowMs);

    // Geometry — public so callers can lay out adjacent UI
    static constexpr uint16_t SPRITE_W = 64;
    static constexpr uint16_t SPRITE_H = 48;
    static constexpr uint16_t SPRITE_X = TFT_WIDTH  - SPRITE_W;   // 176
    static constexpr uint16_t SPRITE_Y = TFT_HEIGHT - SPRITE_H;   // 272

private:
    TFT_eSPI&   _tft;
    TFT_eSprite _spr;

    FaceExpression _expr     = FACE_IDLE;
    uint32_t       _lastDraw = 0;
    bool           _blinking = false;
    uint32_t       _blinkAt  = 0;

    // Timing
    static constexpr uint32_t FPS_MS          = 50;    // 20 fps max
    static constexpr uint32_t BLINK_INTERVAL  = 3500;
    static constexpr uint32_t BLINK_DURATION  = 130;

    // Eye geometry inside the 64×48 sprite
    static constexpr int16_t  EYE_L_X = 18;
    static constexpr int16_t  EYE_R_X = 46;
    static constexpr int16_t  EYE_Y   = 20;
    static constexpr uint16_t EYE_R   = 9;    // iris outer radius
    static constexpr uint16_t PUPIL_R = 4;    // pupil radius

    // Mouth anchor (centre-bottom of the face)
    static constexpr int16_t  MOUTH_X = 32;
    static constexpr int16_t  MOUTH_Y = 38;

    // Palette
    static constexpr uint16_t COL_BG      = 0x0841;  // very dark blue-grey panel
    static constexpr uint16_t COL_IRIS    = 0x4DFB;  // Eilik light blue  (#4CD0F8)
    static constexpr uint16_t COL_ATTACK  = 0xF8C0;  // deep orange-red
    static constexpr uint16_t COL_SUCCESS = 0x07E0;  // bright green
    static constexpr uint16_t COL_MOUTH   = 0xCE59;  // soft white-grey

    void drawFrame();
    void drawEyeOpen  (int16_t cx, int16_t cy, uint16_t irisColor);
    void drawEyeClosed(int16_t cx, int16_t cy, uint16_t color);
    void drawSmile    (int16_t mx, int16_t my, uint16_t color);
    void drawNeutral  (int16_t mx, int16_t my, uint16_t color);
    void drawFrown    (int16_t mx, int16_t my, uint16_t color);
};

#endif // HAS_ILI9341 || HAS_ST7789 || HAS_ST7796
