#include "../include/dummy_target.hpp"
#include <iostream>
#include <cstdlib>

DummyTarget::DummyTarget() {
    current_state = 0;
}

void DummyTarget::reset() {
    current_state = 0;
}

void DummyTarget::receive_packet(const std::string& packet) {
    // Intentional Vulnerability: State Confusion / Auth Bypass
    if (packet == "send_payload" && current_state == 0) {
        std::cerr << "\n[!] VULNERABILITY TRIGGERED: Auth Bypass leading to Crash!" << std::endl;
        std::abort(); 
    }

    // Normal State Machine Logic
    if (current_state == 0 && packet == "send_connect") {
        current_state = 1;
        std::cout << "[Target] State changed to AUTH" << std::endl;
    } 
    else if (current_state == 1 && packet == "send_login") {
        current_state = 2;
        std::cout << "[Target] State changed to DATA" << std::endl;
    }
    else if (current_state == 2 && packet == "send_payload") {
        std::cout << "[Target] Payload processed safely." << std::endl;
    }
    else {
        std::cout << "[Target] Invalid packet for current state. Dropping." << std::endl;
    }
}