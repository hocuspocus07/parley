# Parley: A Coverage-Guided Stateful Protocol Fuzzing Framework

**Parley** is a Proof-of-Concept (PoC) fuzzing framework designed to explore state-aware, conversation-based fuzzing paradigms. Unlike traditional byte-level fuzzers, Parley mutates protocol logic and state sequences to uncover deep semantic vulnerabilities such as Authentication Bypasses and State Confusion.

## The Research Gap

Traditional coverage-guided fuzzers treat inputs as raw byte arrays. While highly effective for stateless parsers (like image or video libraries), they struggle significantly with network protocols. If a target expects a strict sequence of states (e.g., `CONNECT` → `AUTH` → `DATA`), a traditional mutator will likely fail the initial handshake, dropping the packet before ever reaching the core application logic.

**Parley** shifts the abstraction level from *byte arrays* to *conversations*.

## Architecture

Parley is built in modern C++ (C++17) and consists of three core components:

1. **The Grammar Parser:** Uses YAML to define a protocol's Finite State Machine (FSM). This decouples the fuzzer engine from the protocol rules, allowing researchers to swap targets without recompiling the core.
2. **The Semantic Mutator:** Generates a mathematically valid sequence of states by walking the FSM graph, then applies semantic mutations (e.g., packet dropping, state skipping, sequence reordering) to induce logical failures.
3. **The Coverage Engine:** Implements a custom coverage bitmap to track code-block execution. The fuzzer monitors this map after each iteration to detect if a specific mutation unlocked a new execution path.

```text
[YAML Grammar] ➔ [FSM Graph Builder] ➔ [Valid Sequence Generator]
                                                  │
                                                  ▼
[Coverage Bitmap] ◀───────── [Semantic Mutator]
             ▲                                    │
             │                                    ▼
             └──────(Coverage Feedback)────── [Target Harness]
```

## Proof of Concept: Authentication Bypass

To demonstrate the engine's capability, this repository includes a `DummyTarget` harness with an intentional **State Confusion / Auth Bypass** vulnerability. The target crashes if it receives a `send_payload` packet before `send_login`.

Because Parley understands the state machine, it intentionally drops the `send_login` packet while maintaining the rest of the conversation, reliably triggering the vulnerability in milliseconds.

### Example Output

```text
[*] Starting Parley Engine...
[+] Successfully Loaded Protocol: DummyProtocol

[*] Starting Fuzzing Loop...

--- Fuzzing Iteration 1 ---
[Mutator] Randomly dropping packet: send_payload
[Engine] Sending: send_connect
[Target] State changed to AUTH
[Engine] Sending: send_login
[Target] State changed to DATA
[*] System says: NEW PATH UNLOCKED! (Saving state as interesting)

--- Fuzzing Iteration 3 ---
[Mutator] Randomly dropping packet: send_login
[Engine] Sending: send_connect
[Target] State changed to AUTH
[Engine] Sending: send_payload
[Target] Invalid packet for current state. Dropping.
[*] System says: NEW PATH UNLOCKED! (Saving state as interesting)

--- Fuzzing Iteration 9 ---
[Mutator] Randomly dropping packet: send_connect
[Engine] Sending: send_login
[Target] Invalid packet for current state. Dropping.
[Engine] Sending: send_payload

[!] VULNERABILITY TRIGGERED: Auth Bypass leading to Crash!
```

*Note how the engine successfully identifies new code paths via the custom coverage bitmap before finding the fatal state combination.*

## Build Instructions

Parley requires CMake and a C++17 compatible compiler. It uses `yaml-cpp` as its sole external dependency.

```bash
# Clone the repository and its submodules
git clone https://github.com/hocuspocus07/parley.git
cd parley

# Build the project
mkdir build && cd build
cmake ..
cmake --build .

# Run the fuzzer against the dummy target
./Debug/fuzzer.exe  # On Windows
# ./fuzzer          # On Linux/macOS
```

## Future Research Directions

This MVP serves as a foundational prototype for stateful fuzzing research. Future development phases include:

- **LLVM SanitizerCoverage Integration:** Replacing the custom bitmap with `-fsanitize-coverage=trace-pc-guard` to instrument real-world C/C++ targets at compile-time.
- **Value-Level Mutation:** Combining semantic state mutation with traditional byte-flipping (e.g., sending a valid `AUTH` state, but with an overflowed username buffer).
- **Real-World Application:** Building harnesses for lightweight IoT protocols (MQTT, CoAP) to uncover CVEs in open-source brokers.

---

### Next Step

This project demonstrates:

1. Understanding of modern fuzzing concepts such as coverage-guided fuzzing, finite state machines, and coverage feedback.
2. Practical experience with modern C++, CMake, and modular software architecture.
3. A security research mindset focused on uncovering semantic vulnerabilities rather than only memory corruption bugs.