// Levels definitions, functions to evaluate progress, etc

#include "defines.h"

// Current level loaded in RAM for easier access
static level_t level_data;


const static level_t PROGMEM levels[] = {
    //  Nivel   XXXXXXXXXXXXX                    1111111111111    2222222222222    3333333333333    4444444444444    5555555555555
    { /*  0 */ "Puerta NOT",             1, 1, {"Invierte el",   "valor del bit", "de la entrada", "",              ""} },
    { /*  1 */ "Buffer",                 1, 1, {"La salida es",  "siempre igual", "al valor  de",  "la entrada",    ""} },
    { /*  2 */ "Puerta OR",              2, 1, {"La salida es",  "1 si al menos", "una  de  las",  "entradas es 1", ""} },
    { /*  3 */ "Puerta AND",             2, 1, {"La salida da",  "1 cuando las",  "dos entradas",  "son 1",         ""} },
    { /*  4 */ "Puerta XOR",             2, 1, {"La salida da",  "1 cuando las",  "dos entradas",  "son distintas", ""} },
    { /*  5 */ "Puerta NOR",             2, 1, {"La salida da",  "1 cuando las",  "dos entradas",  "son 0",         ""} },
    { /*  6 */ "Permiso 1 bit",          2, 1, {"El primer bit", "en la entrada", "pasa si tiene", "el permiso",    "del bit 2"} },
    { /*  7 */ "Comparador",             2, 1, {"Devuelve 1 si", "las entradas",  "son iguales",   "(puerta XNOR)", ""} },
    { /*  8 */ "Siempre 1",              1, 1, {"La salida da",  "siempre 1",     "sin importar",  "el valor de",   "la entrada"} },
    { /*  9 */ "Selección",              3, 1, {"Selecciona la", "entrada de 1",  "bit que pasa",  "a  la salida",  ""} },
    { /* 10 */ "Suma de 1 bit",          2, 2, {"Suma los dos",  "bits  de  la",  "entrada, y da", "el resultado",  ""} },
    { /* 11 */ "Palíndromo",             3, 1, {"La salida da",  "1 cuando la",   "entrada es un", "palíndromo",    ""} },
    { /* 12 */ "Mayoría de 1s",          3, 1, {"La salida da",  "1 si más de",   "la mitad es 1", "",              ""} },
    { /* 13 */ "Señal Única",            3, 1, {"Devuelve 1 si", "solamente una", "entrada está",  "activa",        ""} },
    { /* 14 */ "Contador",               3, 2, {"Cuenta los 1s", "en la entrada", "y los escribe", "a la salida",   ""} },
    { /* 15 */ "Suma 2 bits",            4, 3, {"Suma los dos ", "números  de 2", "bits, y da el", "resultado",     ""} },
    { /* 16 */ "Decodificador",          2, 4, {"Enciende el",   "bit de salida", "indicado por",  "la entrada",    ""} },
    { /* 17 */ "Selector 2bit",          5, 2, {"Elige cuál de", "las entradas",  "de 2 bits se ", "muestra a la",  "salida" } },
    { /* 18 */ "Iguales o no?",          4, 1, {"La salida da",  "1 cuando las",  "2 entradas de", "2 bits son",    "iguales"} },
    { /* 19 */ "Pasa la mayor",          4, 2, {"Muestra en la", "salida, la",    "entrada de 2",  "bits que sea",  "mayor"  } },
    { /* 20 */ "Pasa la menor",          4, 2, {"Muestra en la", "salida, la",    "entrada de 2",  "bits más",      "pequeña"} },
};  //  Nivel   XXXXXXXXXXXXX                   11111111111112222222222222333333333333344444444444445555555555555

// Evaluation is separated from levels structure, as PROGMEM doesn't work with dynamic expressions
uint8_t evaluateLevelInput(uint8_t level, bits_t input)
{
    bits_t o = 0;
    bits_t i = input & ((1 << level_data.input_bits) - 1);

    switch (level) {
    case 0: o = !i.a; break;                                                    // Gate NOT
    case 1: o = i.a; break;                                                     // Buffer
    case 2: o = (i.a | i.b); break;                                             // Gate OR
    case 3: o = (i.a & i.b); break;                                             // Gate AND
    case 4: o = (i.a ^ i.b); break;                                             // Gate XOR
    case 5: o = ~(i.a | i.b); break;                                            // Gate NOR
    case 6: o = (i.a & i.b); break;                                             // Permiso 1 bit
    case 7: o = (i.a == i.b); break;                                            // Comparator
    case 8: o = (i.a | !i.a); break;                                            // Siempre 1
    case 9: o = (i.c)?i.b:i.a; break;                                           // Selector 1 bit
    case 10: o = (i.a + i.b); break;                                            // Suma de 1 bit
    case 11: o = (i.a == i.c); break;                                           // Palíndromo
    case 12: o = ((i.a & i.b) | (i.b & i.c) | (i.a & i.c)); break;              // Mayoría de 1s
    case 13: o = (i.a&~(i.b|i.c))|(i.b&~(i.a|i.c))|(i.c&~(i.a|i.b)); break;     // Señal Única
    case 14: o = (i.a + i.b + i.c); break;                                      // Contador de bits
    case 15: o = (i.b << 1 | i.a) + (i.d << 1 | i.c); break;                    // Suma de 2 bits
    case 16: o = (1 << (i.b << 1 | i.a)); break;                                // Decodificador
    case 17: o = (i.e)?(i.d<<1)|i.c:(i.b<<1)|i.a; break;                        // Selector 2bit
    case 18: o = (((i.d<<1)|i.c)==((i.b<<1)|i.a))?1:0; break;                   // Iguales o no?
    case 19: o = ((i.d<<1|i.c)>(i.b<<1|i.a))?(i.d<<1|i.c):(i.b<<1|i.a);break;   // Cuál es mayor
    case 20: o = ((i.d<<1|i.c)<(i.b<<1|i.a))?(i.d<<1|i.c):(i.b<<1|i.a);break;   // Cuál es menor
    default: o = 0;
    }
    return o & ((1 << level_data.output_bits) - 1);
}

void initializeLevels()
{
    loadLevel(0);
}

uint8_t getMaximumLevel()
{
    return (sizeof(levels) / sizeof(level_t)) - 1;
}

level_t *loadLevel(uint8_t level)
{
    memcpy_P(&level_data, &levels[level], sizeof(level_t));
    return &level_data;
}

level_t *getLevelData()
{
    return &level_data;
}
