#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "elma/elma.h"

using namespace elma;

class Integrator : public Process {
public:
    Integrator(std::string name) : Process(name) {}

    void init() override {
        _integral = 0.0;
    }

    void update() override {
        if (!channel("link").nonempty()) return;

        double v = channel("link").latest();
        _integral += delta() * v;
    }

    double value() const {
        return _integral;
    }

private:
    double _integral = 0.0;
};

#endif
