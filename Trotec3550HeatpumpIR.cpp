//################ DEBUG SETTINGS ##################
//################################################## 
#define DEBUG 1  // Set to 1 to enable, 0 to disable

#if defined(DEBUG) && (DEBUG > 0)
#define LOG(...) Serial.print(__VA_ARGS__)
#define LOGLN(...) Serial.println(__VA_ARGS__)
#else
#define LOG(...)
#define LOGLN(...)
#endif

#include <Trotec3550HeatpumpIR.h>

Trotec3550HeatpumpIR::Trotec3550HeatpumpIR() : HeatpumpIR()
{
  static const char model[] PROGMEM = "TROTEC3550";
  static const char info[]  PROGMEM = "{\"mdl\":\"trotec3550\",\"dn\":\"TROTEC3550\",\"mT\":16,\"xT\":30,\"fs\":5}";

  _model = model;
  _info = info;
}


void Trotec3550HeatpumpIR::send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, uint8_t fanSpeedCmd, uint8_t temperatureCmd, uint8_t swingVCmd, uint8_t swingHCmd)
{
LOGLN("TROTEC3550HeatpumpIR.cpp" "DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG - Line 101 TROTEC3550HeatpumpIR.cpp");

  // Sensible defaults for the heat pump mode

  uint8_t powerMode = TROTEC3550_AIRCON1_MODE_ON;
  uint8_t operatingMode = TROTEC3550_AIRCON1_MODE_AUTO;
  uint8_t fanSpeed = TROTEC3550_AIRCON1_FAN_AUTO;
  uint8_t temperature = 23;
  uint8_t swingV = TROTEC3550_AIRCON1_VDIR_MANUAL;
  uint8_t swingH = TROTEC3550_AIRCON1_HDIR_MANUAL;

  if (powerModeCmd == POWER_OFF)
  {
    powerMode = TROTEC3550_AIRCON1_MODE_OFF;
  }
  else
  {
    switch (operatingModeCmd)
    {
      case MODE_AUTO:
        operatingMode = TROTEC3550_AIRCON1_MODE_AUTO;
        break;
      case MODE_HEAT:
        operatingMode = TROTEC3550_AIRCON1_MODE_HEAT;
        break;
      case MODE_COOL:
        operatingMode = TROTEC3550_AIRCON1_MODE_COOL;
        break;
      case MODE_DRY:
        operatingMode = TROTEC3550_AIRCON1_MODE_DRY;
        break;
      case MODE_FAN:
        operatingMode = TROTEC3550_AIRCON1_MODE_FAN;
       break;
    }
  }

  switch (fanSpeedCmd)
  {
    case FAN_AUTO:
      fanSpeed = TROTEC3550_AIRCON1_FAN_AUTO;
      break;
    case FAN_1:
      fanSpeed = TROTEC3550_AIRCON1_FAN1;
      break;
    case FAN_2:
      fanSpeed = TROTEC3550_AIRCON1_FAN2;
      break;
    case FAN_3:
      fanSpeed = TROTEC3550_AIRCON1_FAN3;
      break;
  }

  if ( temperatureCmd > 15 && temperatureCmd < 31)
  {
    temperature = temperatureCmd;
  }

  if (swingVCmd == VDIR_SWING)
  {
    swingV = TROTEC3550_AIRCON1_VDIR_SWING;
  }

    if (swingHCmd == HDIR_SWING)
  {
    swingH = TROTEC3550_AIRCON1_HDIR_SWING;
  }

  sendTROTEC3550(IR, powerMode, operatingMode, fanSpeed, temperature, swingV, swingH);
}


void Trotec3550HeatpumpIR::sendTROTEC3550(IRSender& IR, uint8_t powerMode, uint8_t operatingMode, uint8_t fanSpeed, uint8_t temperature, uint8_t swingV, uint8_t swingH)
{
LOGLN("TROTEC3550HeatpumpIR.cpp" "DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG - Line 27 TROTEC3550HeatpumpIR.cpp");  // ON, HEAT, AUTO FAN, +24 degrees
  uint8_t TROTEC3550Template[] = { 0xC3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00 };
  //                           0     1     2     3     4     5     6     7     8     9    10    11    12

  uint8_t checksum = 0x00;

  // Set the power mode on the template message
  TROTEC3550Template[9] |= powerMode;

  // Set the operatingmode on the template message
  TROTEC3550Template[6] |= operatingMode;

  // Set the temperature on the template message
  TROTEC3550Template[1] |= (temperature - 8) << 3;

  // Set the fan speed on the template message
  TROTEC3550Template[4] |= fanSpeed;

  // Set the vertical air direction on the template message
  TROTEC3550Template[1] |= swingV;

  // Set the horizontal air direction on the template message
  TROTEC3550Template[2] |= swingH;

  // Calculate the checksum
  for (uint8_t i = 0; i < 12; i++) {
    checksum += TROTEC3550Template[i];
  }

  TROTEC3550Template[12] = checksum;

  // 38 kHz PWM frequency
  IR.setFrequency(38);

  // Header
  IR.mark(TROTEC3550_AIRCON1_HDR_MARK);
  IR.space(TROTEC3550_AIRCON1_HDR_SPACE);

  // Data
  for (uint8_t i=0; i<sizeof(TROTEC3550Template); i++) {
    IR.sendIRbyte(TROTEC3550Template[i], TROTEC3550_AIRCON1_BIT_MARK, TROTEC3550_AIRCON1_ZERO_SPACE, TROTEC3550_AIRCON1_ONE_SPACE);
  }

  // End mark
  IR.mark(TROTEC3550_AIRCON1_BIT_MARK);
  IR.space(0);
}
