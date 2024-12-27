// Music data, and functions to handle and play sound

#include "defines.h"
#include <rtttl.h>

// Music and sounds
//const char tetris[] PROGMEM = "tetris:d=4,o=5,b=160:e6,8b,8c6,8d6,16e6,16d6,8c6,8b,a,8a,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a";
const char intro[] PROGMEM = "themepark:d=4,o=5,b=160:8b,8a#,16a,16a#,16a,16g#,8g,8f#,8f,8f#,8g#,8g,16f#,16g,16f#,16f,8e,8d#,8d,8d#,8f#,16d#,16e,8c,8c#,8f#,16d#,16e,8c,8c#,16a#4,16b4,16c#,16d#,16e,16f#,16g#,16a#,8b,8f#,8b";
const char startlevel[] PROGMEM = "arkanoid:d=4,o=5,b=140:8g6,16p,16g.6,2a#6,32p,8a6,8g6,8f6,8a6,2g6";
const char levelcompleted[] PROGMEM = "Easy2:d=4,o=5,b=63:8g4,32g,32d,32g,32g,32p,32d,8g";
const char gamecompleted[] PROGMEM ="alliwant:d=4,o=5,b=100:16d6,8d6,16c6,8d6,8f6,16d#6,8d#6,16d6,32d#6,32d6,8c6,16d6,16d6,8d6,16c6,8d6,8f6,16g6,8f6,16f6,16g6,8f6,16f6,16a#6,8a#6,16a#6,g6,16a#6,8a#6,16a#6,16g6,8f6,16d6,8f6,8f6,8f6,8a#6,16a6,8a6,16a6,8a#6,8a6,4a#6";
const char goldenaxe[] PROGMEM = "goldenaxe:d=4,o=5,b=112:8a4,32p,16b4,16p,2c,16p,8a4,32p,16b4,16p,2c,16p,8b4,32p,16c,16p,d,32p,8g.4,16c,16b4,2a4";

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
        case 0: rtttl::begin(buzzerPin, intro); break;
        case 1: rtttl::begin(buzzerPin, startlevel); break;
        case 2: rtttl::begin(buzzerPin, levelcompleted); break;
        case 3: rtttl::begin(buzzerPin, gamecompleted); break;
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
