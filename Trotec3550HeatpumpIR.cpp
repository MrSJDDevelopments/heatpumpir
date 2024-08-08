#define DEBUG
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
  static const char info[]  PROGMEM = "{\"mdl\":\"Trotec3550\",\"dn\":\"TROTEC3550\",\"mT\":16,\"xT\":30,\"fs\":3}";
  LOGLN("");
  _model = model;
  _info = info;
}


void Trotec3550HeatpumpIR::send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, uint8_t fanSpeedCmd, uint8_t temperatureCmd, uint8_t swingVCmd)
{ 
  LOGLN("");
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

void Trotec3550HeatpumpIR::sendTROTEC3550(IRSender& IR, uint8_t powerMode, uint8_t operatingMode, uint8_t fanSpeed, uint8_t temperature, uint8_t swingV)
{
 LOGLN("");
//########################### Code Info Sourced From ############################
//###############################################################################
// https://github.com/crankyoldgit/IRremoteESP8266
// https://github.com/crankyoldgit/IRremoteESP8266/blob/master/src/ir_Trotec.h
// https://github.com/crankyoldgit/IRremoteESP8266/blob/master/src/ir_Trotec.cpp

//############### IR Code Template #################
//################################################## 
  uint8_t TROTEC3550Template[] = { 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  //  Data Bytes                     0     1     2     3     4     5     6     7     8

  uint8_t checksum = 0x00;                                  

//############## Power Mode Location ###############          ################# Code Structure #################        
//##################################################          ################################################## 
  TROTEC3550Template[1] |= powerMode;                         //  Byte 0  //
                                                              //    uint8_t Intro     :8;  <-|-- fixed @ (0x55)
//############ Operating Mode Location #############          //  Byte 1  //
//##################################################          //    uint8_t SwingV    :1;
  TROTEC3550Template[6] |= operatingMode;                     //    uint8_t Power     :1;
                                                              //    uint8_t           :1;  <-|-- Unknown
//########### Temperature in C Location ############          //    uint8_t TimerSet  :1;
//##################################################          //    uint8_t TempC     :4;  <-|-- Temp
  TROTEC3550Template[1] |= (temperature - 8) << 3;            //  Byte 2  //
                                                              //    uint8_t TimerHrs  :4;
//############## Fan Speed Location ################          //    uint8_t           :4;  <-|-- Unknown
//##################################################          //  Byte 3  //
  TROTEC3550Template[6] |= fanSpeed;                          //    uint8_t TempF     :5;  <-|-- Temp
                                                              //    uint8_t           :3;  <-|-- Unknown
//############### Airflow Location #################          //  Byte 4  //
//##################################################          //    uint8_t           :8;  <-|-- Unknown
  TROTEC3550Template[1] |= swingV;                            //  Byte 5  //
                                                              //    uint8_t           :8;  <-|-- Unknown
//############# Checksum Calculation ###############          //  Byte 6  //
//##################################################          //    uint8_t Mode      :2;
  TROTEC3550Template[8] = checksum;                           //    uint8_t           :2;  <-|-- Unknown
                                                              //    uint8_t Fan       :2;
//############### Checksum Location ################          //    uint8_t           :2;  <-|-- Unknown
//##################################################          //  Byte 7  //
   for (uint8_t i = 0; i < 8; i++) {                          //    uint8_t           :7;  <-|-- Unknown
     checksum += TROTEC3550Template[i];                       //    uint8_t Celsius   :1;  <-|-- DegC or DegF                                 
  }                                                           //  Byte 8  //
                                                              //    uint8_t Sum       :8;  <-|-- Checksums
//############### Carrier Frequency ################
//################################################## 
  IR.setFrequency(38);

//################ Protocol Headder ################
//################################################## 
  IR.mark(TROTEC3550_AIRCON1_HDR_MARK);
  IR.space(TROTEC3550_AIRCON1_HDR_SPACE);

//################# Protocol Data ##################
//################################################## 
  for (uint8_t i=0; i<sizeof(TROTEC3550Template); i++) {
    IR.sendIRbyte(TROTEC3550Template[i], TROTEC3550_AIRCON1_BIT_MARK, TROTEC3550_AIRCON1_ZERO_SPACE, TROTEC3550_AIRCON1_ONE_SPACE);
  }

//############### Protocol End Mark ################
//################################################## 
  IR.mark(TROTEC3550_AIRCON1_BIT_MARK);
  IR.space(0);
}
