#include "parser.hpp"
#include <yaml-cpp/yaml.h>
#include <iostream>

ProtocolGraph Parser::parse_grammar(const std::string& filepath) {
    ProtocolGraph graph;
    try {
        // Load the file
        YAML::Node config = YAML::LoadFile(filepath);
        graph.protocol_name = config["protocol"].as<std::string>();

        // Loop through all states
        YAML::Node states_node = config["states"];
        for (std::size_t i = 0; i < states_node.size(); i++) {
            State s;
            s.name = states_node[i]["name"].as<std::string>();

            // Loop through all transitions for this specific state
            YAML::Node trans_node = states_node[i]["transitions"];
            for (std::size_t j = 0; j < trans_node.size(); j++) {
                Transition t;
                t.to_state = trans_node[j]["to"].as<std::string>();
                t.trigger = trans_node[j]["trigger"].as<std::string>();
                s.transitions.push_back(t);
            }
            // Save the state into our map
            graph.states[s.name] = s;
        }
    } catch (const YAML::Exception& e) {
        std::cerr << "YAML Parsing Error: " << e.what() << std::endl;
    }
    return graph;
}