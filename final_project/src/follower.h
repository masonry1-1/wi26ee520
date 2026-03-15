#ifndef __FOLLOWER_AGENT__H
#define __FOLLOWER_AGENT__H

#include <cmath>
#include <string>
#include "enviro.h"

using namespace enviro;

class FollowerController : public Process, public AgentInterface {
    public:
    FollowerController() : Process(), AgentInterface(), leader_x(0), leader_y(0),
                           leader_theta(0), leader_paused(false), slot(1) {}

    void init() {
        watch("leader_state", [&](Event& e) {
            leader_x = e.value()["x"];
            leader_y = e.value()["y"];
            leader_theta = e.value()["theta"];
            leader_paused = e.value()["paused"];
        });
    }

    void start() {
        slot = (id() % 7) + 1;
        label("F" + std::to_string(slot), 14, 14);
    }

    void update() {
        if ( leader_paused ) {
            track_velocity(0, 0);
            label("F" + std::to_string(slot) + " (paused)", 14, 14);
            return;
        }

        double spacing = 48.0;
        double target_x = leader_x - spacing * slot * std::cos(leader_theta);
        double target_y = leader_y - spacing * slot * std::sin(leader_theta);

        double dx = target_x - x();
        double dy = target_y - y();
        double dist = std::sqrt(dx*dx + dy*dy);

        double front = sensor_value(0);
        double front_left = sensor_value(1);
        double front_right = sensor_value(2);

        if ( front < 18 ) {
            track_velocity(-5, (front_left < front_right) ? -0.7 : 0.7);
        } else if ( front_left < 14 ) {
            track_velocity(3, -0.45);
        } else if ( front_right < 14 ) {
            track_velocity(3, 0.45);
        } else if ( dist > 10 ) {
            move_toward(target_x, target_y, 28, 95);
        } else {
            track_velocity(0, 0);
        }

        decorate("<circle cx='0' cy='0' r='2' style='fill: #0d3a73'></circle>");
        label("F" + std::to_string(slot), 14, 14);
    }

    void stop() {}

    private:
    double leader_x, leader_y, leader_theta;
    bool leader_paused;
    int slot;
};

class Follower : public Agent {
    public:
    Follower(json spec, World& world) : Agent(spec, world) {
        add_process(controller);
    }

    private:
    FollowerController controller;
};

DECLARE_INTERFACE(Follower)

#endif
