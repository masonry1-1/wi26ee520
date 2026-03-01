#ifndef FILTER_H
#define FILTER_H

#include <deque>
#include "elma/elma.h"

using namespace elma;

class Filter : public Process {
public:
    Filter(std::string name) : Process(name) {}

    void init() override {
        _sum = 0.0;
        _avg = 0.0;
        _window.clear();
    }

    void update() override {
        if (!channel("link").nonempty()) return;

        double v = channel("link").latest();

        _window.push_back(v);
        _sum += v;

        if (_window.size() > 10) {
            _sum -= _window.front();
            _window.pop_front();
        }

        _avg = _window.empty() ? 0.0 : _sum / _window.size();
    }

    double value() const {
        return _avg;
    }

private:
    std::deque<double> _window;
    double _sum = 0.0;
    double _avg = 0.0;
};

#endif
