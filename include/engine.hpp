#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "parser.hpp"
#include "dummy_target.hpp"
#include <vector>
#include <string>

class Engine {
public:
    Engine(ProtocolGraph graph);
    
    // attacks n times on the target
    void run_fuzzer(DummyTarget& target, int iterations);

private:
    ProtocolGraph protocol_graph;
    
    // reads rulebook and creates a valid sequence
    std::vector<std::string> generate_valid_sequence();
    
    // Semantic mutation
    std::vector<std::string> mutate_sequence(const std::vector<std::string>& original);
};

#endif