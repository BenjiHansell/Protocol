#ifndef HEADER_PACKETS
#define HEADER_PACKETS

#pragma pack(1) // prevents padding of structs to allow accurate (undefined) pointer access

// All packets derive from this class
struct MSP_PACKET{
  public: // might actually make these protected
    MSP_PACKET():last_changed(0){}
    uint8_t message_id;
    uint8_t message_length;
    uint32_t last_changed;  
};

// All packets used for retrieving data from the FC derive from this class
struct MSP_PACKET_GET : public MSP_PACKET{
};

// All packets used for sending data to the FC derive from this class
struct MSP_PACKET_SET : public MSP_PACKET{
};

struct MSP_IDENT : public MSP_PACKET_GET{
  MSP_IDENT(){
    message_id = 100;
    message_length = 7;
  }
  uint8_t version;
  uint8_t multitype;
  uint8_t msp_version;
  uint32_t capability;
};

struct MSP_STATUS : public MSP_PACKET_GET{
  MSP_STATUS(){
    message_id = 101;
    message_length = 11;
  }
  uint16_t cycle_time;
  uint16_t i2c_errors_count;
  uint16_t sensor;
  uint32_t flag;
  uint8_t global_conf;
};

struct MSP_RAW_IMU : public MSP_PACKET_GET{
  MSP_RAW_IMU(){
    message_id = 102;
    message_length = 18;
  }
  int16_t accx;
  int16_t accy;
  int16_t accz;
  int16_t gyrx;
  int16_t gyry;
  int16_t gyrz;
  int16_t magx;
  int16_t magy;
  int16_t magz;
};

struct MSP_SERVO : public MSP_PACKET_GET{
  MSP_SERVO(){
    message_id = 103;
    message_length = 16;
  }
  uint16_t servo[8];
};

struct MSP_MOTOR : public MSP_PACKET_GET{
  MSP_MOTOR(){
    message_id = 104;
    message_length  = 16;
  }
  uint16_t motor[8];
};

struct MSP_RC : public MSP_PACKET_GET{
  MSP_RC(){
    message_id = 105;
    message_length  = 32;
  }
  uint16_t channel[16];
};

struct MSP_RAW_GPS : public MSP_PACKET_GET{
  MSP_RAW_GPS(){
    message_id = 106;
    message_length  = 16;
  }
  uint8_t fix;            // might be able to change this to bool
  uint8_t num_sat;
  uint32_t lattitude;     // 1 / 10 000 000 degree
  uint32_t longitude;     // 1 / 10 000 000 degree
  uint16_t altitude;      // metre
  uint16_t speed;         // cm / s
  uint16_t ground_course; // degree * 10
};

struct MSP_ATTITUDE : public MSP_PACKET_GET{
  MSP_ATTITUDE(){
    message_id = 108;
    message_length = 6;
  }
  int16_t angx;
  int16_t angy;
  int16_t heading;  
};

struct MSP_ANALOG : public MSP_PACKET_GET{
  MSP_ANALOG(){
    message_id = 110;
    message_length = 7;
  }
  uint8_t vbat;
  uint16_t intPowerMeterSum;
  uint16_t rssi;
  uint16_t amperage;
};

struct MSP_SET_RAW_RC : public MSP_PACKET_SET{
  MSP_SET_RAW_RC()
  {
    message_id = 200;
    message_length = 32;
    for (int i = 0; i < 16; i++)
      (&roll)[i] = 1500;
  throttle = 1000;
  }
  uint16_t roll;
  uint16_t pitch;
  uint16_t throttle;
  uint16_t yaw;
  uint16_t aux1;
  uint16_t aux2;
  uint16_t aux3;
  uint16_t aux4;
  uint16_t aux5;
  uint16_t aux6;
  uint16_t aux7;
  uint16_t aux8;
  uint16_t aux9;
  uint16_t aux10;
  uint16_t aux11;
  uint16_t aux12;
};

struct MSP_SET_RAW_GPS : public MSP_PACKET_SET{
  MSP_SET_RAW_GPS()
  {
    message_id = 201;
    message_length  = 16;
  }
  uint8_t fix;            // might be able to change this to bool
  uint8_t num_sat;
  uint32_t lattitude;     // 1 / 10 000 000 degree
  uint32_t longitude;     // 1 / 10 000 000 degree
  uint16_t altitude;      // metre
  uint16_t speed;         // cm / s
  uint16_t ground_course; // degree * 10
};

#endif
