#ifndef HEADER_PROTOCOL
#define HEADER_PROTOCOL

#include "Arduino.h"
#include "Packets.h"

#include <StandardCplusplus.h>
#include <map>

class Protocol{
  public:
    Protocol(Stream& _SerialFC):SerialFC(_SerialFC){} // begin() must be called externally
    void request(MSP_PACKET_GET& p); // requests a packet from FC and stores a pointer to the packet, when the data arrives it (and a timestamp) will be stored in p
    void send(MSP_PACKET_SET& p); // sends data in p to FC and stores pointer to p, when a verification if recieved from FC the timestamp of p will be updated
    void sendAndForget(MSP_PACKET_SET&); // sends data in p to FC but does not store data
    void requestAll();  // requests every packet with one call
    void requestOne();  // requests one of the stored packets, then incriments to the next packet with the nect call
    void addPacket(MSP_PACKET_GET& p); // stores a pointer to p but does not immediately request a packet from FC (to be called once and followed with repeated calls to request(MSP_PACKET_GET&), requestAll() or requestOne()
    void removePacket(MSP_PACKET_GET& p); // if p matches a stored packet, the pointer is deleted and future packets of that type from the FC will be ignored
    void removePacket(MSP_PACKET_SET& p); // if p matches a stored packet, the pointer is deleted and future vericfications of packets of that type from the flight controller will be ignored
  private:
    Stream& SerialFC;
    std::map<uint8_t, MSP_PACKET_GET*> packets_incoming;
    std::map<uint8_t, MSP_PACKET_SET*> packets_outgoing;
    void request(uint8_t type);
    void processIncoming(uint8_t type, uint8_t * payload);
    void sendPacket(uint8_t type, uint8_t size, uint8_t * dat);
    void sort();
};

#endif
