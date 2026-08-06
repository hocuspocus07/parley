#ifndef DUMMY_TARGET_HPP
#define DUMMY_TARGET_HPP

#include <string>
#include <vector>
#include <cstdint>

class DummyTarget {
private:
    int current_state; // 0 = INIT, 1 = AUTH, 2 = DATA

public:
    DummyTarget();
    
    std::vector<uint8_t> coverage_map;
    // Simulates receiving a packet over the network
    void receive_packet(const std::string& packet);
    
    // Resets the server for the next fuzzing iteration
    void reset();
};

#endif