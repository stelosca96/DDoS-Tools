//
// Created by stefano on 12/01/21.
//

#ifndef DDOS_TOOLS_SYNFLOOD_H
#define DDOS_TOOLS_SYNFLOOD_H
#include <tins/tins.h>

using namespace Tins;

class SynFlood {
private:
    const IPv4Address dst_ip;
    const uint16_t dst_port;
    const unsigned counter;
    const unsigned threads_number;
    const unsigned upload_bandwidth;

    static uint16_t random_ip_part();
    static void init_random();
    static IPv4Address random_ip_address();
    static void syn_flood(IPv4Address d_ip, uint16_t d_port, unsigned counter, double upload_bandwidth);

public:
    SynFlood(const IPv4Address &dstIp, uint16_t dstPort, unsigned int counter,
            unsigned int threads_number, unsigned int upload_bandwidth);
    void run();

};


#endif //DDOS_TOOLS_SYNFLOOD_H
