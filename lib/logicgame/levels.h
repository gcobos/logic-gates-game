#ifndef levels_h
#define levels_h

// Level type definition 
typedef struct {
	char name[17];                  // Level name
	uint8_t input_bits;             // Input bits
    uint8_t output_bits;            // Output bits
	char desc[5][17];  				// Descriptions
} level_t;

union bits_t {
    uint8_t data;
    struct {
		unsigned int a : 1; // Bit 0
		unsigned int b : 1; // Bit 1
		unsigned int c : 1; // Bit 2
		unsigned int d : 1; // Bit 3
		unsigned int e : 1; // Bit 4
		unsigned int f : 1; // Bit 5
		unsigned int g : 1; // Bit 6
		unsigned int h : 1; // Bit 7
	};
    bits_t(uint8_t value = 0) : data(value) {}

    bits_t& operator=(uint8_t value) {
        data = value;
        return *this;
    }

    operator uint8_t(void) const {
		return data;

    }
};

void initializeLevels();

uint8_t getMaximumLevel();

level_t * loadLevel(uint8_t level);

level_t *getLevelData();

// This function returns the output expected for an input in a level
uint8_t evaluateLevelInput(uint8_t level, bits_t input);

#endif
