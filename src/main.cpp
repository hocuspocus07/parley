#include <iostream>
#include "parser.hpp"

int main() {
    std::cout << "[*] Starting Parley Engine..." << std::endl;
    
    Parser parser;
    // Load our dummy grammar
    ProtocolGraph graph = parser.parse_grammar("grammars/dummy.yaml");

    std::cout << "[+] Successfully Loaded Protocol: " << graph.protocol_name << "\n" << std::endl;
    
    // Print out what we learned from the YAML
    for (const auto& [name, state] : graph.states) {
        std::cout << "State: " << name << std::endl;
        for (const auto& t : state.transitions) {
            std::cout << "  -> Moves to [" << t.to_state << "] via [" << t.trigger << "]" << std::endl;
        }
        std::cout << "-----------------------" << std::endl;
    }
    
    return 0;
}