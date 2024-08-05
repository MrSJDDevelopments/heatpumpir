/*
    TROTEC heatpump control (remote control ??????)
*/
#ifndef Trotec3550HeatpumpIR_h
#define Trotec3550HeatpumpIR_h

#include <HeatpumpIR.h>


// TROTEC timing constants
#define TROTEC3550_AIRCON1_HDR_MARK    12000 // 9260 <-- Arduino decoder printouts
#define TROTEC3550_AIRCON1_HDR_SPACE   5130 // 4540
#define TROTEC3550_AIRCON1_BIT_MARK    550  // 540
#define TROTEC3550_AIRCON1_ONE_SPACE   1950 // 1640
#define TROTEC3550_AIRCON1_ZERO_SPACE  500  // 480

// TROTEC codes
//#define TROTEC3550_AIRCON1_MODE_AUTO   0x00 // Operating mode
//#define TROTEC3550_AIRCON1_MODE_HEAT   0x80
#define TROTEC3550_AIRCON1_MODE_COOL   0x21
#define TROTEC3550_AIRCON1_MODE_DRY    0x12
#define TROTEC3550_AIRCON1_MODE_FAN    0x23

#define TROTEC3550_AIRCON1_MODE_OFF    0x00 // Power
#define TROTEC3550_AIRCON1_MODE_ON     0x20

//#define TROTEC3550_AIRCON1_FAN_AUTO    0xA0 // Fan speed
#define TROTEC3550_AIRCON1_FAN1        0x11
#define TROTEC3550_AIRCON1_FAN2        0x21
#define TROTEC3550_AIRCON1_FAN3        0x31

#define TROTEC3550_AIRCON1_VDIR_MANUAL 0x00 // Air direction modes
#define TROTEC3550_AIRCON1_VDIR_SWING  0x03

//#define TROTEC3550_AIRCON1_HDIR_MANUAL 0x00
//#define TROTEC3550_AIRCON1_HDIR_SWING  0xE0


class Trotec3550HeatpumpIR : public HeatpumpIR
{
  //uint8_t swingHCmd
  public:
    Trotec3550HeatpumpIR();
    void send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, uint8_t fanSpeedCmd, uint8_t temperatureCmd, uint8_t swingVCmd);

//uint8_t swingH
  private:
    void sendTROTEC3550(IRSender& IR, uint8_t powerMode, uint8_t operatingMode, uint8_t fanSpeed, uint8_t temperature, uint8_t swingV);
};

#endif
