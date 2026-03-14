#include "FaceAnimator.h"

#if defined(HAS_ILI9341) || defined(HAS_ST7789) || defined(HAS_ST7796)

// ============================================================
// Constructor / lifecycle
// ============================================================

FaceAnimator::FaceAnimator(TFT_eSPI& tft) : _tft(tft), _spr(&tft) {}

void FaceAnimator::begin() {
    _spr.createSprite(SPRITE_W, SPRITE_H);
    _spr.setColorDepth(16);
    _blinkAt = millis() + BLINK_INTERVAL;

    // Clear the pet region immediately — no white rectangle on boot
    _tft.fillRect(SPRITE_X, SPRITE_Y, SPRITE_W, SPRITE_H, COL_BG);
}

void FaceAnimator::setExpression(FaceExpression expr) {
    if (_expr == expr) return;
    _expr     = expr;
    _blinking = false;
    _blinkAt  = millis() + BLINK_INTERVAL;
    _lastDraw = 0;  // force immediate redraw
}

// ============================================================
// update() — call every loop() iteration
// ============================================================

void FaceAnimator::update(uint32_t nowMs) {
    if (nowMs - _lastDraw < FPS_MS) return;
    _lastDraw = nowMs;

    // Blink state machine — idle only
    if (_expr == FACE_IDLE) {
        if (!_blinking && nowMs >= _blinkAt) {
            _blinking = true;
        }
        if (_blinking && nowMs >= _blinkAt + BLINK_DURATION) {
            _blinking = false;
            _blinkAt  = nowMs + BLINK_INTERVAL;
        }
    } else {
        _blinking = false;
    }

    drawFrame();
}

// ============================================================
// Primitives — all operate on sprite RAM, no SPI until pushSprite
// ============================================================

void FaceAnimator::drawEyeOpen(int16_t cx, int16_t cy, uint16_t irisColor) {
    // Outer glow ring (dim version of iris colour, 1 step darker)
    _spr.fillCircle(cx, cy, EYE_R + 1, (uint16_t)(irisColor >> 1) & 0x7BEF);
    // Iris
    _spr.fillCircle(cx, cy, EYE_R, irisColor);
    // Pupil
    _spr.fillCircle(cx, cy, PUPIL_R, TFT_BLACK);
    // Specular highlight — top-left
    _spr.fillCircle(cx - 3, cy - 3, 2, TFT_WHITE);
    // Second tiny highlight
    _spr.drawPixel(cx + 3, cy - 4, 0xCE59);
}

void FaceAnimator::drawEyeClosed(int16_t cx, int16_t cy, uint16_t color) {
    // Closed = a thin horizontal dash (squint line)
    _spr.drawFastHLine(cx - EYE_R,     cy,     EYE_R * 2 + 1, color);
    _spr.drawFastHLine(cx - EYE_R + 2, cy + 1, EYE_R * 2 - 3, color);
}

// Gentle U-curve smile — 4 line segments, double-pass for 2px thickness
void FaceAnimator::drawSmile(int16_t mx, int16_t my, uint16_t color) {
    _spr.drawLine(mx - 10, my - 2, mx - 5, my + 1, color);
    _spr.drawLine(mx - 5,  my + 1, mx,     my + 3, color);
    _spr.drawLine(mx,      my + 3, mx + 5, my + 1, color);
    _spr.drawLine(mx + 5,  my + 1, mx + 10, my - 2, color);

    _spr.drawLine(mx - 10, my - 1, mx - 5, my + 2, color);
    _spr.drawLine(mx - 5,  my + 2, mx,     my + 4, color);
    _spr.drawLine(mx,      my + 4, mx + 5, my + 2, color);
    _spr.drawLine(mx + 5,  my + 2, mx + 10, my - 1, color);
}

// Flat line — neutral / thinking
void FaceAnimator::drawNeutral(int16_t mx, int16_t my, uint16_t color) {
    _spr.drawFastHLine(mx - 9, my,     18, color);
    _spr.drawFastHLine(mx - 9, my + 1, 18, color);
}

// Inverted U — frown
void FaceAnimator::drawFrown(int16_t mx, int16_t my, uint16_t color) {
    _spr.drawLine(mx - 10, my + 2, mx - 5, my - 1, color);
    _spr.drawLine(mx - 5,  my - 1, mx,     my - 3, color);
    _spr.drawLine(mx,      my - 3, mx + 5, my - 1, color);
    _spr.drawLine(mx + 5,  my - 1, mx + 10, my + 2, color);

    _spr.drawLine(mx - 10, my + 1, mx - 5, my - 2, color);
    _spr.drawLine(mx - 5,  my - 2, mx,     my - 4, color);
    _spr.drawLine(mx,      my - 4, mx + 5, my - 2, color);
    _spr.drawLine(mx + 5,  my - 2, mx + 10, my + 1, color);
}

// ============================================================
// drawFrame() — compose sprite and push in a single SPI burst
// ============================================================

void FaceAnimator::drawFrame() {
    // Panel background — rounded dark rectangle
    _spr.fillSprite(TFT_BLACK);
    _spr.fillRoundRect(0, 0, SPRITE_W, SPRITE_H, 8, COL_BG);

    switch (_expr) {

    // ---- IDLE: blue eyes, smile, periodic blink -------------------------
    case FACE_IDLE:
        if (_blinking) {
            drawEyeClosed(EYE_L_X, EYE_Y, COL_IRIS);
            drawEyeClosed(EYE_R_X, EYE_Y, COL_IRIS);
        } else {
            drawEyeOpen(EYE_L_X, EYE_Y, COL_IRIS);
            drawEyeOpen(EYE_R_X, EYE_Y, COL_IRIS);
        }
        drawSmile(MOUTH_X, MOUTH_Y, COL_MOUTH);
        break;

    // ---- ATTACK: orange-red eyes, squint, frown -------------------------
    case FACE_ATTACK:
        drawEyeOpen(EYE_L_X, EYE_Y, COL_ATTACK);
        drawEyeOpen(EYE_R_X, EYE_Y, COL_ATTACK);
        // Squint: erase top portion of each iris with background
        _spr.fillRect(EYE_L_X - EYE_R - 1, EYE_Y - EYE_R - 1,
                      (EYE_R + 1) * 2 + 2,  (EYE_R / 2) + 3, COL_BG);
        _spr.fillRect(EYE_R_X - EYE_R - 1, EYE_Y - EYE_R - 1,
                      (EYE_R + 1) * 2 + 2,  (EYE_R / 2) + 3, COL_BG);
        drawFrown(MOUTH_X, MOUTH_Y, COL_MOUTH);
        break;

    // ---- SUCCESS: green arc eyes, big smile ----------------------------
    case FACE_SUCCESS:
        // Arc eye = top half of circle filled, drawn as upward curve
        for (int16_t x = -(int16_t)EYE_R; x <= (int16_t)EYE_R; x++) {
            int16_t dy = (int16_t)sqrtf((float)(EYE_R * EYE_R) - (float)(x * x));
            // outer arc
            _spr.drawPixel(EYE_L_X + x, EYE_Y - dy, COL_SUCCESS);
            _spr.drawPixel(EYE_R_X + x, EYE_Y - dy, COL_SUCCESS);
            // thicken
            if (x > -(int16_t)(EYE_R - 1) && x < (int16_t)(EYE_R - 1)) {
                int16_t dy2 = (int16_t)sqrtf((float)((EYE_R - 2) * (EYE_R - 2)) - (float)(x * x));
                _spr.drawPixel(EYE_L_X + x, EYE_Y - dy2, COL_SUCCESS);
                _spr.drawPixel(EYE_R_X + x, EYE_Y - dy2, COL_SUCCESS);
            }
        }
        drawSmile(MOUTH_X, MOUTH_Y - 1, COL_SUCCESS);
        break;
    }

    // One SPI burst — no tearing
    _spr.pushSprite(SPRITE_X, SPRITE_Y);
}

#endif // HAS_ILI9341 || HAS_ST7789 || HAS_ST7796
