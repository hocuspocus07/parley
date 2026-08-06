#include "../include/dummy_target.hpp"
#include <iostream>
#include <cstdlib>

DummyTarget::DummyTarget() {
    current_state = 0;
    coverage_map.assign(10, 0); //array of 10 zeros
}

void DummyTarget::reset() {
    current_state = 0;
    coverage_map.assign(10, 0); //reset back
}

void DummyTarget::receive_packet(const std::string& packet) {
    coverage_map[0] = 1; // Block 0: Packet received

    if (packet == "send_payload" && current_state == 0) {
        coverage_map[1] = 1; // Block 1: Vulnerability Path Hit!
        std::cerr << "\n[!] VULNERABILITY TRIGGERED: Auth Bypass leading to Crash!" << std::endl;
        std::abort(); 
    }

    if (current_state == 0 && packet == "send_connect") {
        coverage_map[2] = 1; // Block 2: Connect Path
        current_state = 1;
        std::cout << "[Target] State changed to AUTH" << std::endl;
    } 
    else if (current_state == 1 && packet == "send_login") {
        coverage_map[3] = 1; // Block 3: Login Path
        current_state = 2;
        std::cout << "[Target] State changed to DATA" << std::endl;
    }
    else if (current_state == 2 && packet == "send_payload") {
        coverage_map[4] = 1; // Block 4: Safe Payload Path
        std::cout << "[Target] Payload processed safely." << std::endl;
    }
    else {
        coverage_map[5] = 1; // Block 5: Error Path
        std::cout << "[Target] Invalid packet for current state. Dropping." << std::endl;
    }
}