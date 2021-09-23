#include <Arduino.h>
#include <EEPROM.h>
#include <CRCx.h>
#include "config.h"

/** Store default settings */
void saveConf(Config conf)
{
    // calc crc32
    uint32_t crc32 = crcx::crc32((uint8_t *)&conf, sizeof(conf) - 4);
    conf.crc32 = crc32;

    // save it
    EEPROM.begin(EES);
    EEPROM.put(0, conf);
    EEPROM.commit();
    EEPROM.end();
}

// load credentials from EEPROM
void loadConf(Config conf)
{
    EEPROM.begin(EES);
    EEPROM.get(0, conf);
    EEPROM.end();

    // calcule crc32
    uint32_t crc32 = crcx::crc32((uint8_t *)&conf, sizeof(conf) - 4);

    if (crc32 != conf.crc32)
    {
        // debug
#ifdef DEBUG
        Serial.println("Bad CRC on EEPROM, loading defaults.");
#endif

        conf.azdratio = DEF_AZDRATIO;
        conf.eldratio = DEF_ELDRATIO;
        conf.azpulses = 0;
        conf.elpulses = 0;

        // save defaults
        saveConf(conf);
    }
}