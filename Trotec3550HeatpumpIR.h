/*
    TROTEC3550 heatpump control (remote control YKR-N/002E, YKR-P/002E)
*/
#ifndef Trotec3550HeatpumpIR_h
#define Trotec3550HeatpumpIR_h

#include <HeatpumpIR.h>


//################ Code IR Timeings ################
//################################################## 
#define TROTEC3550_AIRCON1_HDR_MARK    12000  // 12062  <-|
#define TROTEC3550_AIRCON1_HDR_SPACE   5130   // 5130   <-|
#define TROTEC3550_AIRCON1_BIT_MARK    550    // 544    <-|-- Arduino decoder printouts
#define TROTEC3550_AIRCON1_ONE_SPACE   1950   // 1950   <-|
#define TROTEC3550_AIRCON1_ZERO_SPACE  500    // 500    <-|

//################ IR Remote Codes #################
//################################################## 
#define TROTEC3550_AIRCON1_MODE_OFF    0x00  //    <------|-- AC Power
#define TROTEC3550_AIRCON1_MODE_ON     0x02
#define TROTEC3550_AIRCON1_MODE_COOL   0x11
#define TROTEC3550_AIRCON1_MODE_FAN    0x13  //    <------|-- AC Mode
#define TROTEC3550_AIRCON1_MODE_DRY    0x12
#define TROTEC3550_AIRCON1_FAN1        0x11
#define TROTEC3550_AIRCON1_FAN2        0x21  //    <------|-- AC Fan Speed
#define TROTEC3550_AIRCON1_FAN3        0x31
#define TROTEC3550_AIRCON1_VDIR_SWING  0x03  //    <------|-- AC Air Direction


class TROTEC3550HeatpumpIR : public HeatpumpIR
{
  public:
    TROTEC3550HeatpumpIR();
    void send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, uint8_t fanSpeedCmd, uint8_t temperatureCmd, uint8_t swingVCmd);

  private:
    void sendTROTEC3550(IRSender& IR, uint8_t powerMode, uint8_t operatingMode, uint8_t fanSpeed, uint8_t temperature, uint8_t swingV);
};

#endif
