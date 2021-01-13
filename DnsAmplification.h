//
// Created by stefano on 12/01/21.
//

#ifndef DDOS_TOOLS_DNSAMPLIFICATION_H
#define DDOS_TOOLS_DNSAMPLIFICATION_H
#include <tins/tins.h>
#include <vector>
using namespace Tins;

// todo: limitare banda in upload
class DnsAmplification {
private:
    const IPv4Address dst_ip;
    const std::vector<IPv4Address> dns_servers;
    const unsigned counter;
    const unsigned threads_number;
    const unsigned upload_bandwidth;

    static void dns_amplification(IPv4Address target_ip, const IPv4Address dns_server,
                                  unsigned int counter, double upload_bandwidth);
    static void init_random();
public:
    DnsAmplification(const IPv4Address &dstIp, std::vector<IPv4Address> dnsServers, unsigned int upload_size,
                     unsigned int threadsNumber, unsigned int upload_bandwidth);
    void run();

};


#endif //DDOS_TOOLS_DNSAMPLIFICATION_H
