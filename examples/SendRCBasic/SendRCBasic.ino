#include <Protocol.h>

Protocol protocol(Serial);   //  choose which serial port to use for MSP 
MSP_SET_RAW_RC rc;            //  rc will hold all rc values

void setup() {
  Serial.begin(115200);
}

void loop() {
  rc.roll = 1100; // rc values are on a scale 1000-2000
  rc.pitch = 1200;
  rc.yaw = 1300;
  rc.throttle = 1000;
  rc.aux12 = 1500+500*sin(millis()/1000.0);
  protocol.send(rc); // send the data
  delay(40);      // a flight controller could be overloaded by overly-frequent packets
}
