
#include <stdio.h>
#include <cmath>
#include <iostream>


void printBinary8(unsigned char num, unsigned char term = '\n') {
    for (int i = 7; i >= 0; i--) {
        putchar((num & (1 << i)) ? '1' : '0');
    }
	if (term) putchar(term);
}

using namespace std;

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

    operator uint8_t() const {
        return data;
    }

	bits_t operator++(int) {
		return data++;
	}
};

int main(int argc, char* argv[])
{

	unsigned int level = 0;
	unsigned char input_bits = 1;
	unsigned char i, o;

	if (argc > 1) level = std::stoi(argv[1]);

	if (argc > 2) input_bits = std::stoi(argv[2]);

	for (bits_t i = 0; i < pow(2, input_bits); i++) {

    switch (level) {
    case 0: o = ~i.a; break;                                                    // Gate NOT
    case 1: o = i.a; break;                                                     // Buffer
    case 2: o = (i.a | i.b); break;                                             // Gate OR
    case 3: o = (i.a & i.b); break;                                             // Gate AND
    case 4: o = (i.a ^ i.b); break;                                             // Gate XOR
    case 5: o = ~(i.a | i.b); break;                                            // Gate NOR
    case 6: o = (i.a & i.b); break;                                             // Permiso 1 bit
    case 7: o = (i.a == i.b); break;                                            // Comparator
    case 8: o = (i.a | ~i.a); break;                                            // Siempre 1
    case 9: o = (i.c)?i.b:i.a; break;                                           // Selector 1 bit
    case 10: o = (i.a + i.b); break;                                            // Suma de 1 bit
    case 11: o = (i.a == i.c); break;                                           // Palíndromo
    case 12: o = (i.a & i.b | i.b & i.c | i.a & i.c); break;                    // Mayoría de 1s
    case 13: o = (i.a&~(i.b|i.c))|(i.b&~(i.a|i.c))|(i.c&~(i.a|i.b)); break;     // Señal Única
    case 14: o = (i.a + i.b + i.c); break;                                      // Contador de bits
    case 15: o = (i.b << 1 | i.a) + (i.d << 1 | i.c); break;                    // Suma de 2 bits
    case 16: o = (1 << (i.b << 1 | i.a)); break;                                // Decodificador
    case 17: o = ((i.d<<1|i.c)>(i.b<<1|i.a))?(i.d<<1|i.c):(i.b<<1|i.a); break;// Cuál es mayor
    case 18: o = (i.e)?(i.d<<1|i.c):(i.b<<1|i.a);break;                         // Selector 2bit
    case 19: o = (i.e)?(i.d<<1|i.c):(i.b<<1|i.a);break;                         // Selector 2bit
    default: o = 0;
    }

		cout << "On level " << level << ", for input=";
		printBinary8(i, 0);
		cout << ", the output is ";
		printBinary8(o);
		//cout << endl;
	}
}