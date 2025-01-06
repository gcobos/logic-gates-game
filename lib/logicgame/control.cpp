// Tools for game control: Encoder, inputs and outputs, etc
#include "defines.h"

// Variables to keep the current and last state
volatile static uint8_t encoderPosition = 0;
static uint8_t lastEncoderPosition = 0; // Store the last reported position
static uint8_t encoderMinimum = 0;
static uint8_t encoderMaximum = 5;
static volatile uint8_t levelInput = 0;

volatile bool encoderReady = true;
volatile bool buttonWasPressed = false;

void initializeControl() 
{
    // Set encoder pins as input with pull-up resistors
    pinMode(encoderPinA, INPUT_PULLUP);
    pinMode(encoderButtonPin, INPUT_PULLUP); 
    pinMode(encoderPinB, INPUT_PULLUP);

    // Attach interrupts to the encoder pins
    attachPinChangeInterrupt(digitalPinToPCINT(encoderPinA), updateEncoderPosition, CHANGE);
    attachPinChangeInterrupt(digitalPinToPCINT(encoderButtonPin), encoderButtonPressed, FALLING);
    attachPinChangeInterrupt(digitalPinToPCINT(encoderPinB), updateEncoderPosition, CHANGE);

    // Setup shift registers
    pinMode(SPIDisable, OUTPUT);   // Disable outputs
    digitalWrite(SPIDisable, HIGH); // Disable shift register's outputs
    pinMode(SPILatchPin, OUTPUT);  // Latch (store)
    pinMode(SPIDataPin, OUTPUT);   // Data in
    pinMode(SPIClockPin, OUTPUT);  // Clock (shift)
    pinMode(SPIInputPin, INPUT);   // Input from buttons (not used now)

    SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
    setShiftRegistersOutput();
    digitalWrite(SPIDisable, LOW); // Enable shift register's outputs

    // Setup inputs (logic circuit's output)
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A6, INPUT);
    pinMode(A7, INPUT);

    //attachInterrupt(digitalPinToInterrupt(SPIInputPin), shiftRegisterPinRead, RISING);
}

bool onEncoderPositionChanged()
{
    if (encoderPosition != lastEncoderPosition) {
        lastEncoderPosition = encoderPosition;
#ifdef DEBUG        
        Serial.print(F("Pos: "));
        Serial.println(encoderPosition);
#endif
        return true;
    }
    return false;
}

bool onEncoderButtonPressed()
{
    if (buttonWasPressed) {
        buttonWasPressed = false;
        return true;
    }
    return false;
}

void updateEncoderPosition()
{
    uint8_t MSB = digitalRead(encoderPinA); // MSB = most significant bit
    uint8_t LSB = digitalRead(encoderPinB); // LSB = least significant bit
#ifdef DEBUG_ENCODER
    Serial.print(MSB);
    Serial.print(LSB);
    Serial.print("->");
    Serial.println(micros());
#endif
    if (encoderReady) {
        if (!MSB && LSB) {
            encoderPosition++;
            if (encoderPosition > encoderMaximum) encoderPosition = encoderMinimum;
        } else if (!LSB && MSB) {
            encoderPosition--;
            if ((int8_t)encoderPosition < (int8_t)encoderMinimum) encoderPosition = encoderMaximum;
        }
        encoderReady = false;
    }
    if (MSB && LSB) {
        encoderReady = true;
    }
}

void encoderButtonPressed ()
{
    uint8_t pressed = digitalRead(encoderButtonPin);
    if (!pressed) {
        //for (int i=0; i<100; i++) delayMicroseconds(50);
        buttonWasPressed = true;
    }
}

uint8_t getEncoderPosition()
{
    return encoderPosition;
}

void setEncoderPosition(uint8_t pos)
{
    encoderPosition = pos;
}

void setEncoderMargins(uint8_t min, uint8_t max)
{
    encoderMinimum = min;
    encoderMaximum = max;
}

void setShiftRegistersOutput(uint8_t input, uint8_t output, uint8_t check)
{
    regs.inputs = input;
    regs.outputs = output;
    regs.checks = check;
    SPI.transfer16(regs.data);     // Input and output
    digitalWrite(SPILatchPin, HIGH);
    digitalWrite(SPILatchPin, LOW);
    PORTB |= (1 << PB2); PORTB &= ~(1 << PB2);
}

uint8_t getCircuitOutput()
{
    register uint8_t v = PINC;
    return ((v&0b11000000)>>2)|(v&0b1111);
}

uint8_t evaluateLevelProgress(uint8_t level)
{
    register uint8_t i, expected, actual;
    uint8_t progress = 0;
    uint8_t zeroes = 0;
    uint8_t input_bits = getLevelData()->input_bits;
    regs.outputs = 0;
    regs.checks = 0;
    for (i = 0; i < (1 << input_bits); i++) {
        expected = evaluateLevelInput(level, i);
        regs.inputs = i;
        regs.outputs = expected;
        SPI.transfer16(regs.data);
        PORTB |= (1 << PB2); PORTB &= ~(1 << PB2);
        // Read all analog inputs (circuit's output) removing A4 and A5
        // actual = getCircuitOutput();
        actual = ((PINC&0b11000000)>>2)|(PINC&0b1111);
#ifdef DEBUG
        Serial.print(F("For input "));
        Serial.print(i, BIN);
        Serial.print(F(", expected out = "));
        Serial.print(expected, BIN);
        Serial.print(F(", actual out = "));
        Serial.println(actual, BIN);
#endif
        if (!actual) zeroes++;
        progress += (expected ^ actual)? 0 : 1;
    }
    progress = uint8_t(100.0 * progress / i);
    if (zeroes == i) progress = 0;
#ifdef DEBUG
    Serial.print(F("Progress: "));
    Serial.println(progress);
#endif
    return progress;
}

void setPowerSaveMode(bool state)
{
    setDisplayPowerSave(state);
    if (state) {
        setShiftRegistersOutput(0, 0);
        set_sleep_mode(SLEEP_MODE_STANDBY);
        sleep_enable();
        sleep_mode();
        delay(5);
    } else {

    }
    sleep_disable();
}