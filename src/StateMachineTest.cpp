#include <iostream>

struct run{};
struct do_clean{};
struct turn{};
struct move{};

constexpr auto establish = []{ std::puts("establish"); };
constexpr auto close = []{ std::puts("close"); };
constexpr auto is_valid = [](auto const&) { return true; };
constexpr auto reset_timeout = []{ std::puts("reset_timeout"); };

class Connection {
    enum class State : char/*std::byte*/ {
        DOCLEAN,
        TURN,
        MOVE,
        RUN,
    } state;

public:
    constexpr void process_event(run const&) {
        switch(state) {
            default: break;
            case State::RUN:
                establish(); state = State::CONNECTING; break;
        }
    }

    constexpr void process_event(do_clean const&) {
        switch(state) {
            default: break;
            case State::CONNECTING:
            case State::CONNECTED: close(); state = State::DISCONNECTED; break;
        }
    }

    constexpr void process_event(turn const&) {
        if (state == State::CONNECTING) {
            state = State::CONNECTED;
        }
    }

    constexpr void process_event(move const& event) {
        if (state == State::CONNECTED and is_valid(event)) {
            reset_timeout();
        }
    }

    constexpr void process_event(timeout const&) {
        if (state == State::CONNECTED) {
            establish();
            state = State::CONNECTING;
        }
    }
};