#ifndef CONFIG_H
#define CONFIG_H

/******** DEBUG FLAG **********/
#define DEBUG 1

/************ TCP/IP settings ****************/
#define TCP_PORT 4533   // default for rotctld

/************ PIN definitions ****************/
// Outputs movement
#define MRIGHT  14 // D5
#define MLEFT   12 // D6
#define MUP     13 // D7
#define MDOWN   15 // D8

// Input movement interrupts (change)
#define IIAZ 10 // SD3
#define IIEL  9 // SD2

// movement current sensor
#define AZLIMIT 5 // D1
#define ELLIMIT 4 // D2

// Rotor transformer relay
#define TRAFOON 0 // D3

// Polar coordinates limits
#define MAXAZIMUTH 360.0
#define MINAZIMUTH 0.0
#define MAXELEVATION 72.0
#define MINELEVATION 0.0

// EEPROM size
#define EES 22

// config Struct
typedef struct
{
    float azdratio;
    float eldratio;
    int azpulses;
    int elpulses;
    uint32_t crc32;
} Config;

// autosave interval (in seconds)
#define AUTOSAVE_INTERVAL 30

// default values for config (only re az/el delta ratio)
#define DEF_AZDRATIO 45.0;
#define DEF_ELDRATIO 22.5;

#endif // CONFIG_H