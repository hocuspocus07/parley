#include "engine.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

Engine::Engine(ProtocolGraph graph) : protocol_graph(graph) {
    std::srand(std::time(nullptr)); // init randomizer
    global_coverage.assign(10, 0); // Start tracking from zero
}

// 1. create perfectly valid conversation
std::vector<std::string> Engine::generate_valid_sequence() {
    std::vector<std::string> sequence;
    std::string current_state = "INIT";
    
    // INIT to last state loop
    while (true) {
        if (protocol_graph.states.find(current_state) == protocol_graph.states.end() || 
            protocol_graph.states[current_state].transitions.empty()) {
            break; // last state
        }
        
        // choose the first available transition
        Transition t = protocol_graph.states[current_state].transitions[0];
        sequence.push_back(t.trigger);
        current_state = t.to_state;
    }
    return sequence;
}

// mutators: drops packets randomly
std::vector<std::string> Engine::mutate_sequence(const std::vector<std::string>& original) {
    std::vector<std::string> mutated = original;
    
    if (mutated.size() > 1) {
        int drop_index = std::rand() % mutated.size();
        std::cout << "[Mutator] Randomly dropping packet: " << mutated[drop_index] << std::endl;
        mutated.erase(mutated.begin() + drop_index);
    }
    
    return mutated;
}

bool Engine::has_new_coverage(const std::vector<uint8_t>& run_coverage) {
    bool is_new = false;
    for (size_t i = 0; i < run_coverage.size(); ++i) {
        if (run_coverage[i] == 1 && global_coverage[i] == 0) {
            global_coverage[i] = 1; // save new coverage in global memory
            is_new = true;
        }
    }
    return is_new;
}

// 3. Main Fuzzing Loop, continuously attacks the target with mutated sequences
void Engine::run_fuzzer(DummyTarget& target, int iterations) {
    for (int i = 0; i < iterations; ++i) {
        std::cout << "\n--- Fuzzing Iteration " << i + 1 << " ---" << std::endl;
        
        target.reset(); // back to init
        
        std::vector<std::string> valid_seq = generate_valid_sequence();
        std::vector<std::string> mutated_seq = mutate_sequence(valid_seq);

        // Mutated sequence sent to target
        for (const std::string& packet : mutated_seq) {
            std::cout << "[Engine] Sending: " << packet << std::endl;
            target.receive_packet(packet);
        }

        if (has_new_coverage(target.coverage_map)) {
            std::cout << "[*] System says: NEW PATH UNLOCKED! (Saving state as interesting)" << std::endl;
        }
    }
}