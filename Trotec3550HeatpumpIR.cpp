#include <Trotec3550HeatpumpIR.h>

Trotec3550HeatpumpIR::Trotec3550HeatpumpIR() : HeatpumpIR()
{
  static const char model[] PROGMEM = "TROTEC3550";
  static const char info[]  PROGMEM = "{\"mdl\":\"TROTEC3550\",\"dn\":\"TROTEC3550\",\"mT\":16,\"xT\":30,\"fs\":3}";

  _model = model;
  _info = info;
}


void TROTEC3550HeatpumpIR::send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, uint8_t fanSpeedCmd, uint8_t temperatureCmd, uint8_t swingVCmd)
{
//############### DEFAULT SETTINGS #################
//################################################## 

  uint8_t powerMode = TROTEC3550_AIRCON1_MODE_ON;
  uint8_t operatingMode = TROTEC3550_AIRCON1_MODE_COOL;
  uint8_t fanSpeed = TROTEC3550_AIRCON1_FAN2;
  uint8_t temperature = 18;
  uint8_t swingV = TROTEC3550_AIRCON1_VDIR_SWING;

  if (powerModeCmd == POWER_OFF)
  {
    powerMode = TROTEC3550_AIRCON1_MODE_OFF;
  }
  else
  {
    switch (operatingModeCmd)
    {
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

  if ( temperatureCmd > 16 && temperatureCmd < 30)
  {
    temperature = temperatureCmd;
  }

  if (swingVCmd == VDIR_SWING)
  {
    swingV = TROTEC3550_AIRCON1_VDIR_SWING;
  }

  sendTROTEC3550(IR, powerMode, operatingMode, fanSpeed, temperature, swingV);
}


void TROTEC3550HeatpumpIR::sendTROTEC3550(IRSender& IR, uint8_t powerMode, uint8_t operatingMode, uint8_t fanSpeed, uint8_t temperature, uint8_t swingV)
{
//############### IR Code Template #################
//################################################## 
  uint8_t TROTEC3550Template[] = { 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  //                                 0     1     2     3     4     5     6     7     8

  uint8_t checksum = 0x00;

//############## Power Mode Location ###############
//################################################## 
  TROTEC3550Template[1] |= powerMode;

//############ Operating Mode Location #############
//################################################## 
  TROTEC3550Template[6] |= operatingMode;

//########### Temperature in C Location ############
//################################################## 
  TROTEC3550Template[1] |= (temperature - 8) << 3;

//############## Fan Speed Location ################
//################################################## 
  TROTEC3550Template[6] |= fanSpeed;

//############### Airflow Location #################
//################################################## 
  TROTEC3550Template[1] |= swingV;

//############### Checksum Location ################
//################################################## 
  TROTEC3550Template[8] = checksum;

//############# Checksum Calculation ###############
//################################################## 
  for (uint8_t i = 0; i < 8; i++) {
    checksum += TROTEC3550Template[i];
  }

//############### Carrier Frequency ################
//################################################## 
  TROTEC3550Template[8] = checksum;
  IR.setFrequency(38);

//################ Protocol Headder ################
//################################################## 
  TROTEC3550Template[8] = checksum;
  IR.mark(TROTEC3550_AIRCON1_HDR_MARK);
  IR.space(TROTEC3550_AIRCON1_HDR_SPACE);

//################# Protocol Data ##################
//################################################## 
  TROTEC3550Template[8] = checksum;
  for (uint8_t i=0; i<sizeof(TROTEC3550Template); i++) {
    IR.sendIRbyte(TROTEC3550Template[i], TROTEC3550_AIRCON1_BIT_MARK, TROTEC3550_AIRCON1_ZERO_SPACE, TROTEC3550_AIRCON1_ONE_SPACE);
  }
  
//############### Protocol End Mark ################
//################################################## 
  TROTEC3550Template[8] = checksum;
  IR.mark(TROTEC3550_AIRCON1_BIT_MARK);
  IR.space(0);
}
