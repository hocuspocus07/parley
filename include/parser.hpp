#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <map>

// Represents a single jump from one state to another
struct Transition {
    std::string to_state;
    std::string trigger;
};

// Represents a state (INIT, AUTH) and where we can go from it
struct State {
    std::string name;
    std::vector<Transition> transitions;
};

// The full memory representation of yaml file
struct ProtocolGraph {
    std::string protocol_name;
    std::map<std::string, State> states;
};

class Parser {
public:
    // takes YAML file path and returns the loaded Graph
    ProtocolGraph parse_grammar(const std::string& filepath);
};

#endif