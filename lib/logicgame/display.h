#ifndef display_h
#define display_h

#include <U8g2lib.h>

#include "../../gfx/welcome.xbm"
#include "../../gfx/levels.xbm"
#include "../../gfx/playing.xbm"
#include "../../gfx/complete.xbm"

void initializeDisplay();

char* toBin(uint8_t v, uint8_t len);

void showWelcomeScreen();

void showLevelSelectionScreen(uint8_t level, bool redraw = true);

void showLevelPlayingScreen(uint8_t level, uint8_t progress = 0, bool redraw = true);

void showLevelCompletionScreen(uint8_t level, bool redraw = true);

void setDisplayPowerSave(bool state);

#endif