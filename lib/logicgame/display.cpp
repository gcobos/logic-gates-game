// Pictures for the screen, font selection, GUI, etc
#include "defines.h"

#ifdef DISPLAY_FULL
U8G2_SH1106_128X64_NONAME_F_HW_I2C display(U8G2_R1, A5, A4, U8X8_PIN_NONE);
#define USE_REDRAW
#else
U8G2_SH1106_128X64_NONAME_2_HW_I2C display(U8G2_R1, A5, A4, U8X8_PIN_NONE);
#endif

char buf[10] = "";      // For printing values

void initializeDisplay()
{
    if (!display.begin()) {
#ifdef DEBUG
        Serial.println(F("Screen allocation failed"));
#endif
    }
    display.setBusClock(500000);
    display.enableUTF8Print();
}

char* toBin(uint8_t v, uint8_t len = 5) {
    for (int i = len - 1; i >= 0; i--) {
        buf[i] = '0' + (v & 1);
        v >>= 1;
    }
    buf[len] = '\0';
    return buf;
}

void showWelcomeScreen()
{
    //display.clear();
    display.firstPage();
    do {
        // Export as XBM and use this header: const unsigned char face1_bits[] U8X8_PROGMEM = {
        display.drawXBMP(0, 0, 64, 128, welcome_bits);
    } while (display.nextPage());
}

void showLevelSelectionScreen(uint8_t level, bool redraw)
{
    display.firstPage();
    do {
        display.drawXBMP(0, 0, 64, 128, levels_bits);
        display.setFont(u8g2_font_7x13B_mn);
        display.setCursor(50, 29);
        sprintf(buf, "%02u", level);
        display.print(buf);
        //display.setDrawColor(0);
        //display.drawBox(0, 30, 64, 110);
        //display.setDrawColor(1);
        display.setFont(u8g2_font_5x8_mf);
        // display.setCursor(0, 57);
        display.setCursor(32 - (display.getUTF8Width(getLevelData()->name) / 2), 57);
        display.print(getLevelData()->name);
        for (uint8_t i = 0; i < 5; i++) {
            display.setCursor(0, 89 + 9*i);
            display.print(getLevelData()->desc[i]);
        }
    } while (display.nextPage());
}


void showLevelPlayingScreen(uint8_t level, uint8_t progress, bool redraw)
{
    display.firstPage();
    do {
        display.drawXBMP(0, 0, 64, 128, playing_bits);
        display.setFont(u8g2_font_7x13B_mn);
        display.setCursor(50, 29);
        sprintf(buf, "%02u", level);
        display.print(buf);
        display.setCursor(15, 89);
        display.print(toBin(getCircuitOutput()));
        display.setCursor(15, 119);
        display.print(toBin(getEncoderPosition()));
        display.setFont(u8g2_font_5x8_mf);
        display.setCursor(32 - (display.getUTF8Width(getLevelData()->name) / 2), 57);
        display.print(getLevelData()->name);
        display.drawFrame(0, 94, 64, 12);
        display.drawBox(0, 95, progress * 64 / 100, 10);
        sprintf(buf, "%d%%", progress);
        display.setCursor(32 - strlen(buf)*2.5, 103);
        display.print(buf);
    } while (display.nextPage());
}

void showLevelCompletionScreen(uint8_t level, bool redraw)
{
    display.firstPage();
    do {
        display.drawXBMP(0, 0, 64, 128, complete_bits);
        display.setFont(u8g2_font_7x13B_mn);
        display.setCursor(45, 12);
        sprintf(buf, "%02u", level);
        display.print(buf);
        //display.setDrawColor(0);
        //display.drawBox(0, 30, 64, 110);
        //display.setDrawColor(1);
        display.setFont(u8g2_font_5x8_mf);
        // display.setCursor(0, 57);
        display.setCursor(32 - (display.getUTF8Width(getLevelData()->name) / 2), 57);
        display.print(getLevelData()->name);
        display.setCursor(0, 90);
        display.println(F("¡Enhorabuena!"));
        display.setCursor(0, 105);
        display.println(F("Pulsa para el"));
        display.setCursor(0, 117);
        display.println(F("próximo nivel"));
    } while (display.nextPage());
}
