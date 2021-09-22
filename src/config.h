#ifndef CONFIG_H
#define CONFIG_H

/************ TCP/IP settings ****************/
#define TCP_PORT 4533   // default for rotctld

/************ PIN definitions ****************/
// Outputs movement
#define MRIGHT D5
#define MLEFT D6
#define MUP D7
#define MDOWN D8

// Input movement interrupts
#define IIAZ SD3
#define IIEL SD2

// movement current sensor
#define ISMOVING D1

// Rotor transformer relay
#define TRAFOON D2

#endif // CONFIG_H
