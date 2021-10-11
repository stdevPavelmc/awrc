#ifndef CONFIG_H
#define CONFIG_H

/******** DEBUG FLAG **********/
#define DEBUG 1

/************ TCP/IP settings ****************/

#define HOSTNAME "awrc"
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

/************* Coordinate limits *************/

// Polar coordinates limits
const float MAXAZIMUTH = 360.0;
const byte MINAZIMUTH = 0;
const float MAXELEVATION = 72.0;
const byte MINELEVATION = 0.0;

// default values for config (only re az/el delta ratio)
#define DEF_AZDRATIO 8.777777778  // 3160 pulses/360 degrees
#define DEF_ELDRATIO 17.575342466 // 1283 pulses/72 degrees

// minimum error to track
const float MINERROR = 5.0;

// time limits to know we are stopped (millis)
#define AZSTOPT 1000
#define ELSTOPT 1000

/***************** EEPROM **********************/

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

// State of the system
enum States
{
    IDLE,
    TRACKING,
    MANUAL,
    CALIBRATING,
    PARKING,
    OTA
};

// Web commands
enum WebCmds
{
    NONE,
    CAL,
    STOP,
    PARK,
    RIGHT,
    LEFT,
    UP,
    DOWN
};

#endif // CONFIG_H