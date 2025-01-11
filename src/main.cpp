#include <TimerOne.h>
#include "defines.h"

static volatile bool tick = false;
static unsigned long ticksInState = 0;
static GameState currentState = STATE_WELCOME;

static uint8_t levelInput;
static uint8_t currentLevel;

void setup()
{

#ifdef DEBUG
    Serial.begin(9600);
#endif

    initializeControl();
    initializeSounds();
    initializeDisplay();
    initializeLevels();

    // Global timer, to avoid running in loop all the time
    Timer1.initialize(50000); // Tick every 50ms
    Timer1.attachInterrupt([](){ tick = true; });
}

// MAIN LOOP

void loop() {

    static uint8_t progress;

    if (tick) {
        tick = false;
        /*if (digitalRead(4)) {
            digitalWrite(4, LOW);
        } else {
            digitalWrite(4, HIGH);
        }*/

        // Play music / sounds
        soundTick();

        switch (currentState) {
            case STATE_WELCOME:
                if (ticksInState == 0) {
                    levelInput = 0b01010101;
                    startPlaying(0);
                    showWelcomeScreen();
                    setShiftRegistersOutput(levelInput, ~levelInput);
                } else if (ticksInState < 20 * 6 && ticksInState % 10 == 0) {
                    levelInput = ~levelInput;
                    setShiftRegistersOutput(levelInput, ~levelInput);
                } else if (ticksInState > 20 * 6) {     // 6 secs
                    levelInput = 0;
                    setShiftRegistersOutput(levelInput, levelInput);
                    currentState = STATE_LEVEL_SELECTION;
                    ticksInState = -1;
                }
                break;
            case STATE_LEVEL_SELECTION:
                if (ticksInState == 0) {
#ifdef DEBUG
                    Serial.println(F("Showing level selection screen"));
#endif
                    //digitalWrite(4, HIGH);
                    loadLevel(currentLevel);
                    setEncoderPosition(currentLevel);
                    setEncoderMargins(0, getMaximumLevel());
                    showLevelSelectionScreen(currentLevel);
                }
                if (onEncoderPositionChanged()) {
                    setPowerSaveMode(false);
                    currentLevel = getEncoderPosition();
                    loadLevel(currentLevel);
                    showLevelSelectionScreen(currentLevel, false);
#ifdef DEBUG
                    Serial.println(F("Refreshing level selection screen"));
#endif
                    ticksInState = 0;
                }
                // Allow to go to play the level only after 0.5s
                if (onEncoderButtonPressed() && ticksInState > 10) {
#ifdef DEBUG
                    Serial.print(F("Button pressed at pos: "));
                    Serial.println(getEncoderPosition());
#endif
                    setPowerSaveMode(false);
                    currentState = STATE_PLAYING;
                    ticksInState = -1;
                    startPlaying(1);
                }
                break;
            case STATE_PLAYING:
                if (ticksInState == 0) {
#ifdef DEBUG
                    Serial.println(F("Showing playing level screen"));
#endif
                    levelInput = 0;
                    progress = evaluateLevelProgress(currentLevel);
                    setEncoderPosition(levelInput);
                    setShiftRegistersOutput(levelInput, evaluateLevelInput(currentLevel, levelInput));
                    setEncoderMargins(0, (1 << getLevelData()->input_bits) - 1);
                    showLevelPlayingScreen(currentLevel, progress);
                }
                // Allow to transition back to level selection after 1.5s
                if (onEncoderButtonPressed() && ticksInState >= 30) {
                    setPowerSaveMode(false);
                    currentState = STATE_LEVEL_SELECTION;
                    setEncoderPosition(currentLevel);
                    setShiftRegistersOutput(0, 0);
                    ticksInState = -1;
                    break;
                }
                // Turn input and output LED's accordingly if encoder is moved
                if (onEncoderPositionChanged()) {
                    setPowerSaveMode(false);
                    levelInput = getEncoderPosition();
                    progress = evaluateLevelProgress(currentLevel);
                    setShiftRegistersOutput(levelInput, evaluateLevelInput(currentLevel, levelInput));
                    showLevelPlayingScreen(currentLevel, progress, false);
                    ticksInState = 20;
                }
                // Check completion and refresh screen every 2s
                // If the level is completed (all inputs and outputs match expected on level)
                if (ticksInState > 60 && ticksInState % 40 == 0) {
                    progress = evaluateLevelProgress(currentLevel);
                    setShiftRegistersOutput(levelInput, evaluateLevelInput(currentLevel, levelInput));
                    showLevelPlayingScreen(currentLevel, progress, false);

                    // If progress is completed
                    if (progress == 100) {
                        setPowerSaveMode(false);
                        setShiftRegistersOutput(0, 0);
                        currentState = STATE_COMPLETED;
                        ticksInState = -1;
                        startPlaying(2);
                    }
                }
                break;
            case STATE_COMPLETED:
                // Waits still while the music plays, and then offers to play next level
                if (ticksInState == 20) {
                    showLevelCompletionScreen(currentLevel);
                    currentLevel++;
                    if (currentLevel > getMaximumLevel()) {
                        currentLevel = 0;
                        startPlaying(3);
                    }
                }
                if (onEncoderPositionChanged()) {
                    setPowerSaveMode(false);
                }
                if (onEncoderButtonPressed() && ticksInState > 40) {
#ifdef DEBUG
                    Serial.print(F("Button pressed at pos: "));
                    Serial.println(getEncoderPosition());
#endif
                    setPowerSaveMode(false);
                    currentState = STATE_LEVEL_SELECTION;
                    ticksInState = -1;
                }
                break;
        }
        ticksInState++;
        if (ticksInState == 20 * 20) {  // 20 seconds of inactivity
            setPowerSaveMode(true);
        }
    }
    delay(2);       // Power nap 2ms
}
