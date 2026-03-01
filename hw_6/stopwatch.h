#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>

class Stopwatch {
public:
    Stopwatch() { reset(); }

    void start() {
        if (!_running) {
            _running = true;
            _start = clock::now();
        }
    }

    void stop() {
        if (_running) {
            auto now = clock::now();
            _elapsed += std::chrono::duration_cast<std::chrono::nanoseconds>(now - _start);
            _running = false;
        }
    }

    void reset() {
        _running = false;
        _elapsed = std::chrono::nanoseconds(0);
        _start = clock::now();
    }

    double get_nanoseconds() {
        return static_cast<double>(elapsed().count());
    }

    double get_milliseconds() {
        return get_nanoseconds() / 1e6;
    }

    double get_seconds() {
        return get_nanoseconds() / 1e9;
    }

    double get_minutes() {
        return get_seconds() / 60.0;
    }

private:
    using clock = std::chrono::high_resolution_clock;

    bool _running = false;
    clock::time_point _start;
    std::chrono::nanoseconds _elapsed{0};

    std::chrono::nanoseconds elapsed() {
        if (!_running) return _elapsed;
        auto now = clock::now();
        return _elapsed + std::chrono::duration_cast<std::chrono::nanoseconds>(now - _start);
    }
};

#endif
