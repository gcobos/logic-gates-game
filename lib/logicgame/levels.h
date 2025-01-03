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
		bool a : 1; // Bit 0
		bool b : 1; // Bit 1
		bool c : 1; // Bit 2
		bool d : 1; // Bit 3
		bool e : 1; // Bit 4
		bool f : 1; // Bit 5
		bool g : 1; // Bit 6
		bool h : 1; // Bit 7
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
