#include <iostream>
#include "parser.hpp"
#include "dummy_target.hpp"
#include "engine.hpp"

int main() {
    std::cout << "[*] Starting Parley Engine..." << std::endl;
    
    Parser parser;
    // 1. Load our dummy grammar
    ProtocolGraph graph = parser.parse_grammar("grammars/dummy.yaml");

    std::cout << "[+] Successfully Loaded Protocol: " << graph.protocol_name << "\n" << std::endl;
    
    // 2. Initialize Target & Engine
    DummyTarget target;
    Engine engine(graph);

    std::cout << "[*] Starting Fuzzing Loop...\n" << std::endl;
    
    // 3. run the fuzzer 10 times(until crash is found)
    engine.run_fuzzer(target, 10);
    
    std::cout << "\n[*] Fuzzing finished without crash (if you see this, run again!)." << std::endl;
    
    return 0;
}