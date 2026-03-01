#include <iostream>
#include "stopwatch.h"

int main() {
    Stopwatch w;
    w.start();
    w.stop();
    std::cout << "Elapsed ns: " << w.get_nanoseconds() << "\n";
    return 0;
}
