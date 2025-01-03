#ifndef control_h
#define control_h

#include <SPI.h>
#include <PinChangeInterrupt.h>

volatile static union {
  uint16_t data;
  struct {
    uint8_t outputs:5;      // Outputs (red)
    uint8_t inputs:5;       // Inputs (red)
    uint8_t checks:5;       // Checks (green)
  };
} regs;


void initializeControl();

bool onEncoderPositionChanged();

bool onEncoderButtonPressed();

uint8_t getEncoderPosition();

void setEncoderPosition(uint8_t pos);

void setEncoderMargins(uint8_t min, uint8_t max);

void updateEncoderPosition();

void encoderButtonPressed();

void setShiftRegistersOutput(uint8_t input = 0, uint8_t output = 0, uint8_t check = 0);

// void shiftRegisterPinRead();

// This function returns the output of the circuit for the current input
uint8_t getCircuitOutput();

// This function returns a number between 0 and 100, with 0 meaning "not started"
// and 255 to indicate that the level is completed. For that, it needs to evaluate
// the outputs of the board, for every combination of the input of the level, and
// compare it against the expected values for the same inputs
uint8_t evaluateLevelProgress(uint8_t level);

#endif