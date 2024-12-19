#ifndef defines_h
#define defines_h

#include <Arduino.h>

// Uncomment to enable serial debug (but it may cause issue because of lack of RAM!)
//#define DEBUG

#define DISPLAY_FULL      // Uncomment if using the full buffer

// Define pin for the buzzer
constexpr uint8_t PROGMEM buzzerPin = 3;

// Define the pins used for the encoder
constexpr uint8_t PROGMEM encoderPinA = 5;
constexpr uint8_t PROGMEM encoderButtonPin = 6;
constexpr uint8_t PROGMEM encoderPinB = 7;

// For shift registers test
constexpr uint8_t PROGMEM SPILatchPin = 10;  // Show output
constexpr uint8_t PROGMEM SPIDataPin = 11;
constexpr uint8_t PROGMEM SPIClockPin = 13;  // Shift bit
constexpr uint8_t PROGMEM SPIInputPin = 2;

// Game states
typedef enum {
    STATE_WELCOME,          // Welcome state
    STATE_LEVEL_SELECTION,  // Level selection menu
    STATE_PLAYING,          // Playing the level
    STATE_COMPLETED,        // Level completed
} GameState;

// Include the rest of modules
#include "control.h"
#include "levels.h"
#include "display.h"
#include "sounds.h"

#endif