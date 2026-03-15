#ifndef __COORDINATOR_AGENT__H
#define __COORDINATOR_AGENT__H

#include <string>
#include "enviro.h"

using namespace enviro;

class CoordinatorController : public Process, public AgentInterface {
    public:
    CoordinatorController() : Process(), AgentInterface(), spawned(0) {}

    void init() {
        watch("button_click", [&](Event& e) {
            if ( e.value()["name"] == "add_follower" ) {
                double x = -320 - 40 * (spawned % 5);
                double y = 180 - 30 * (spawned % 4);
                add_agent("Follower", x, y, 0, {
                    {"fill", "#9ec9ff"},
                    {"stroke", "black"}
                });
                spawned++;
            }
        });
    }

    void start() {}
    void update() {}
    void stop() {}

    private:
    int spawned;
};

class Coordinator : public Agent {
    public:
    Coordinator(json spec, World& world) : Agent(spec, world) {
        add_process(controller);
    }

    private:
    CoordinatorController controller;
};

DECLARE_INTERFACE(Coordinator)

#endif
