// Levels definitions, functions to evaluate progress, etc

#include "defines.h"

// Current level loaded in RAM for easier access
static level_t level_data;


const static level_t PROGMEM levels[] = {
    //  Nivel   XXXXXXXXXXXXX                    1111111111111    2222222222222    3333333333333    4444444444444    5555555555555      Solutions
    { /*  0 */ "Puerta NOT",             1, 1, {"Invierte el",   "valor del bit", "de la entrada", "",              ""} },              // 1 (NOT)
    { /*  1 */ "Buffer",                 1, 1, {"La salida es",  "siempre igual", "al valor  de",  "la entrada",    ""} },              // 1 (AND), 1 (OR), 2 (NOT, NOT)
    { /*  2 */ "Puerta OR",              2, 1, {"La salida es",  "1 si al menos", "una  de  las",  "entradas es 1", ""} },              // 1 (OR)
    { /*  3 */ "Puerta AND",             2, 1, {"La salida da",  "1 cuando las",  "dos entradas",  "son 1",         ""} },              // 1 (AND)
    { /*  4 */ "Puerta XOR",             2, 1, {"La salida da",  "1 cuando las",  "dos entradas",  "son distintas", ""} },              // 1 (XOR)
    { /*  5 */ "Puerta NOR",             2, 1, {"La salida da",  "1 cuando las",  "dos entradas",  "son 0",         ""} },              // 2 (OR, NOT)
    { /*  6 */ "Puerta NAND",            2, 1, {"La salida da",  "siempre uno",   "excepto si la", "entrada es",    "igual a 11"} },    // 2 (AND, NOT)
    { /*  7 */ "Permiso 1 bit",          2, 1, {"El primer bit", "de la entrada", "pasa si tiene", "permiso del",   "segundo bit"} },   // 1 (AND)
    { /*  8 */ "Comparador",             2, 1, {"Devuelve 1 si", "las entradas",  "son iguales",   "(puerta XNOR)", ""} },              // 2 (XOR, NOT)
    { /*  9 */ "Siempre 1",              1, 1, {"La salida da",  "siempre 1",     "sin importar",  "el valor de",   "la entrada"} },    // 2 (NOT, XOR)
    { /* 10 */ "Semáforo",               2, 1, {"La salida da",  "1 si el bit 2", "vale 1 y el",   "bit 1 vale 0.", "La entrada=10"} }, // 2 (NOT, AND)
    { /* 11 */ "¿Es par?",               2, 1, {"Devuelve 1 si", "el número de",  "la entrada es", "par (termina",  "en 0)"} },         // 1 (NOT)
    { /* 12 */ "¿Menos que 2?",          2, 1, {"Devuelve 1 si", "el número de",  "la entrada es", "menor que 2",   "(00 ó 01)"} },     // 1 (NOT)
    { /* 13 */ "¿Es impar?",             2, 1, {"Devuelve 1 si", "el número de",  "la entrada es", "impar (acaba",  "en 1)"} },         // 1 (AND), 1 (OR)
    { /* 14 */ "Contraseña",             3, 1, {"La salida da",  "1 solamente",   "cuando la",     "entrada es",    "igual a 101"} },   // 3 (AND, NOT, AND)
    { /* 15 */ "Selección",              3, 1, {"El bit 3 de",   "la entrada",    "decide cuál",   "bit pasará a",  "la salida"} },     // 4 (NOT, AND, AND, OR)
    { /* 16 */ "Suma de 1 bit",          2, 2, {"Suma los dos",  "bits  de  la",  "entrada, y da", "el resultado",  ""} },              // 2 (AND, XOR)
    { /* 17 */ "Palíndromo",             3, 1, {"La salida da",  "1 cuando la",   "entrada es un", "palíndromo",    ""} },              // 2 (XOR, NOT)
    { /* 18 */ "Mayoría de 1s",          3, 1, {"La salida da",  "1 si más de",   "la mitad de",   "bits de la",    "entrada es 1"} },  // 5 (AND, AND, AND, OR, OR)
    { /* 19 */ "Señal Única",            3, 1, {"Devuelve 1 si", "solamente una", "entrada está",  "activa",        ""} },              // 7 (XOR, NOT, XOR, NOT, AND, AND, OR)
    { /* 20 */ "Contador",               3, 2, {"Escribe en la", "salida la",     "cantidad de",   "unos (1's) de", "la entrada"} },    //
    { /* 21 */ "Suma 2 bits",            4, 3, {"Suma los dos",  "números  de 2", "bits, y da el", "resultado",     ""} },              //
    { /* 22 */ "Decodificador",          2, 4, {"Enciende el",   "bit de salida", "indicado por",  "la entrada",    ""} },              //
    { /* 23 */ "Selector 2bit",          5, 2, {"Elige cuál de", "las entradas",  "de 2 bits se",  "muestra a la",  "salida" } },       //
    { /* 24 */ "Iguales o no?",          4, 1, {"La salida da",  "1 cuando las",  "2 entradas de", "2 bits son",    "iguales"} },       //
    { /* 25 */ "Pasa la mayor",          4, 2, {"Muestra en la", "salida, la",    "entrada de 2",  "bits que sea",  "mayor"  } },       //
    { /* 26 */ "Pasa la menor",          4, 2, {"Muestra en la", "salida, la",    "entrada de 2",  "bits más",      "pequeña"} },       //
};  //  Nivel   XXXXXXXXXXXXX                    1111111111111    2222222222222    3333333333333    4444444444444    5555555555555

// Evaluation is separated from levels structure, as PROGMEM doesn't work with dynamic expressions
uint8_t evaluateLevelInput(uint8_t level, bits_t input)
{
    bits_t o = 0;
    bits_t i = input & ((1 << level_data.input_bits) - 1);

    switch (level) {
    case 0: o = !i.a; break;                                                    // Puerta NOT
    case 1: o = i.a; break;                                                     // Buffer
    case 2: o = (i.a | i.b); break;                                             // Puerta OR
    case 3: o = (i.a & i.b); break;                                             // Puerta AND
    case 4: o = (i.a ^ i.b); break;                                             // Puerta XOR
    case 5: o = ~(i.a | i.b); break;                                            // Puerta NOR
    case 6: o = ~(i.a & i.b); break;                                            // Puerta NAND
    case 7: o = (i.a & i.b); break;                                             // Permiso 1 bit (AND)
    case 8: o = (i.a == i.b); break;                                            // Comparador
    case 9: o = (i.a | !i.a); break;                                            // Siempre 1
    case 10: o = (i.a & !i.b); break;                                           // Semáforo (10 -> 1)
    case 11: o = !i.a; break;                                                   // Es par?
    case 12: o = (i.data < 2); break;                                           // Es menos que 2?
    case 13: o = i.a; break;                                                    // Es impar?
    case 14: o = (i.a & !i.b & i.c); break;                                     // Contraseña (es 101)
    case 15: o = (i.c)?i.b:i.a; break;                                          // Selector 1 bit
    case 16: o = (i.a + i.b); break;                                            // Suma de 1 bit
    case 17: o = (i.a == i.c); break;                                           // Palíndromo
    case 18: o = ((i.a & i.b) | (i.b & i.c) | (i.a & i.c)); break;              // Mayoría de 1s
    case 19: o = (i.a&~(i.b|i.c))|(i.b&~(i.a|i.c))|(i.c&~(i.a|i.b)); break;     // Señal Única
    case 20: o = (i.a + i.b + i.c); break;                                      // Contador de bits
    case 21: o = (i.b << 1 | i.a) + (i.d << 1 | i.c); break;                    // Suma de 2 bits
    case 22: o = (1 << (i.b << 1 | i.a)); break;                                // Decodificador
    case 23: o = (i.e)?(i.d<<1)|i.c:(i.b<<1)|i.a; break;                        // Selector 2bit
    case 24: o = (((i.d<<1)|i.c)==((i.b<<1)|i.a))?1:0; break;                   // Iguales o no?
    case 25: o = ((i.d<<1|i.c)>(i.b<<1|i.a))?(i.d<<1|i.c):(i.b<<1|i.a);break;   // Cuál es mayor
    case 26: o = ((i.d<<1|i.c)<(i.b<<1|i.a))?(i.d<<1|i.c):(i.b<<1|i.a);break;   // Cuál es menor
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
