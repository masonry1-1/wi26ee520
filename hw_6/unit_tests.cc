#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>
#include "stopwatch.h"

int main() {
    Stopwatch w;
    w.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    w.stop();

    assert(w.get_seconds() > 0.9 && w.get_seconds() < 1.1);
    w.reset();
    assert(w.get_seconds() == 0.0);

    std::cout << "Stopwatch tests passed.\n";
}
