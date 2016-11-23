#include "Protocol.h"

void Protocol::removePacket(MSP_PACKET_GET& p) {
  if (packets_incoming.count(p.message_id))
    if (packets_incoming[p.message_id] == &p) // indexing of a non-existent map would create an element
      packets_incoming.erase(p.message_id);
}

void Protocol::removePacket(MSP_PACKET_SET& p) {
  if (packets_outgoing.count(p.message_id))
    if (packets_outgoing[p.message_id] == &p)
      packets_outgoing.erase(p.message_id);
}

void Protocol::send(MSP_PACKET_SET& p) {
  sendAndForget(p);
  packets_outgoing[p.message_id] = &p;
}

void Protocol::sendAndForget(MSP_PACKET_SET& p) {
  sendPacket(p.message_id, p.message_length, &(&p.message_id)[6]);
}

void Protocol::sort() {
  const uint8_t sizeMax = 7;
  static byte in[256];
  static uint8_t pos;
  static uint8_t type;
  static uint8_t size;
  static uint8_t sum;
  static uint8_t sum2;

  //  Serial.println("new loop");
  while (SerialFC.available()) {
    //    Serial.println(pos);
    byte next = SerialFC.read();
    switch (pos) {
      case 0:
        next == 0x24 ? pos++ : pos = 0;
        //        debug.println("0x24");
        break;
      case 1:
        next == 0x4D ? pos++ : pos = 0;
        //        debug.println("0x4D");
        break;
      case 2:
        next == 0x3E ? pos++ : pos = 0;
        break;
      case 3:
        if (size <= sizeMax) {
          size = next;
          sum2 = next; // combine into 1 line?
          pos++;
        } else {
          pos = 0;
        }
        break;
      case 4:
        type = next;
        sum2 ^= next;
        pos++;
        break;
      default:
        if (pos - 5 < size) {
          pos++;
          in[pos - 6] = next;
          //          Serial.print("next = "); Serial.println(next);
          sum2 ^= next;
        } else {
          sum = next;
          //          Serial.print("sum\t");
          //          Serial.println(sum, HEX);
          //          Serial.print("sum2\t");
          //          Serial.println(sum2, HEX);
          pos = 0;
          if (sum == sum2) {
            processIncoming(type, in);
          } else {
            //            Serial.println("Unmatched checksums");
          }
        }

        break;
    }
  }
}

void Protocol::processIncoming(uint8_t type, uint8_t * payload) {
  //  Serial.print("Valid packet detected\t");
  if (type >= 100 && type < 200) {
    if (packets_incoming.count(type)) {
      uint8_t size = (packets_incoming[type])->message_length;
      for (int i = 6; i < 6 + size; i++)
        (&(packets_incoming[type]->message_id))[i] = payload[i - 6];
      packets_incoming[type]->last_changed = millis();
    }
  } else if (type >= 200) {
    if (packets_outgoing.count(type)) {
      packets_outgoing[type]->last_changed = millis();
    }
  }
  //  switch (type){
  //    case 110: // MSP_ANALOG
  //      vbat = payload[0];
  //      MSP_ANALOG = millis();
  //      break;
  //    case 200: // MSP_SET_RAW_RC
  //      MSP_SET_RAW_RC = millis();
  //      break;
  //    case 108: // MSP_ATTITUDE
  //      MSP_ATTITUDE = millis();
  //      angx = combine(payload[1], payload[0]);
  //      angy = combine(payload[3], payload[2]);
  //      heading = combine(payload[5], payload[4]);
  //      break;
  //    default:
  ////      Serial.print("Unrecognised packet code ");
  ////      Serial.println(type);
  //      break;
  //  }
}

//void Protocol::sendControl(int8_t size, uint16_t * control){ // RPTY
//  uint8_t data[32];
//  for (int i = 0; i < size; i++){
//    data[2*i] = control[i] & 255;
//    data[2*i+1] = control[i] >> 8 & 255;
//  }
//  sendPacket(200, size*2, data);
//  sort();
//}

void Protocol::requestAll() {
  for (std::map<uint8_t, MSP_PACKET_GET*>::iterator it = packets_incoming.begin(); it != packets_incoming.end(); it++)
    request(it->second->message_id);
}

void Protocol::requestOne() {
  static std::map<uint8_t, MSP_PACKET_GET*>::iterator it = packets_incoming.begin();
  if (packets_incoming.size()) {
    request(it->second->message_id);
    it++;
    if (it == packets_incoming.end()) it = packets_incoming.begin();
  }
}

void Protocol::request(MSP_PACKET_GET& p) {
  if (!packets_incoming.count(p.message_id)) packets_incoming[p.message_id] = &p;
  request(p.message_id);
}

void Protocol::request(uint8_t type) {
  sort();
  uint8_t size = 0; // unnecessary
  SerialFC.write('$');
  SerialFC.write('M');
  SerialFC.write('<');
  SerialFC.write(size);
  SerialFC.write(type);
  SerialFC.write(type);
  sort();
}

void Protocol::addPacket(MSP_PACKET_GET& p) {
  //  Serial.println("addPacket called"); // for some reason this prints a second time
  packets_incoming[p.message_id] = &p;
}

void Protocol::sendPacket(uint8_t type, uint8_t size, uint8_t * dat) { // change "type" to "id"
  byte checksum = 0;
  SerialFC.write('$');
  SerialFC.write('M');
  SerialFC.write('<');
  SerialFC.write(size);
  checksum ^= size;
  SerialFC.write(type);
  checksum ^= type;
  SerialFC.write(dat, size);
  for (int i = 0; i < size; i++) {
    checksum ^= dat[i];
  }
  SerialFC.write(checksum);
}

//int16_t Protocol::combine(int8_t msb, int8_t lsb){
//  return (msb << 8 ) | (lsb & 0xff);
//}

