#ifndef RANDOM_PROCESS_H
#define RANDOM_PROCESS_H

#include <random>
#include "elma/elma.h"

using namespace elma;

class RandomProcess : public Process {
public:
    RandomProcess(std::string name) : Process(name) {}

    void init() override {
        std::random_device rd;
        _rng.seed(rd());
        _dist = std::uniform_real_distribution<double>(0.0, 1.0);
    }

    void update() override {
        double v = _dist(_rng);
        channel("link").send(v);
    }

private:
    std::mt19937_64 _rng;
    std::uniform_real_distribution<double> _dist;
};

#endif
