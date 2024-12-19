// Music data, and functions to handle and play sound

#include "defines.h"
#include <rtttl.h>

// Music and sounds
const char tetris[] PROGMEM = "tetris:d=4,o=5,b=160:e6,8b,8c6,8d6,16e6,16d6,8c6,8b,a,8a,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a";
const char goldenaxe[] PROGMEM = "goldenaxe:d=4,o=5,b=112:8a4,32p,16b4,16p,2c,16p,8a4,32p,16b4,16p,2c,16p,8b4,32p,16c,16p,d,32p,8g.4,16c,16b4,2a4";
const char arkanoid[] PROGMEM = "arkanoid:d=4,o=5,b=140:8g6,16p,16g.6,2a#6,32p,8a6,8g6,8f6,8a6,2g6";
const char easy[] PROGMEM = "Easy2:d=4,o=5,b=63:8g4,32g,32d,32g,32g,32p,32d,8g";

void initializeSounds()
{
    pinMode(buzzerPin, OUTPUT);
    digitalWrite(buzzerPin, LOW);
}

bool isSoundPlaying()
{
    return rtttl::isPlaying();
}

void startPlaying (uint8_t track)
{
    switch (track) {
        case 0: rtttl::begin(buzzerPin, goldenaxe); break;
        case 1: rtttl::begin(buzzerPin, tetris); break;
        case 2: rtttl::begin(buzzerPin, arkanoid); break;
        case 3: rtttl::begin(buzzerPin, easy); break;
        default: rtttl::begin(buzzerPin, goldenaxe);
    }
}

void stopPlaying ()
{
    rtttl::stop();
}

void soundTick() {
    rtttl::play();
}
