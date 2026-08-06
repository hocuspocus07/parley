#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "parser.hpp"
#include "dummy_target.hpp"
#include <vector>
#include <string>
#include <cstdint>

class Engine {
public:
    Engine(ProtocolGraph graph);
    
    // attacks n times on the target
    void run_fuzzer(DummyTarget& target, int iterations);

private:
    ProtocolGraph protocol_graph;

    std::vector<uint8_t> global_coverage;//to check what is new
    // reads rulebook and creates a valid sequence
    std::vector<std::string> generate_valid_sequence();
    
    std::vector<std::string> mutate_sequence(const std::vector<std::string>& original);
    
    // compares coverage
    bool has_new_coverage(const std::vector<uint8_t>& run_coverage);
};

#endif