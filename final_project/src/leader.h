#ifndef __LEADER_AGENT__H
#define __LEADER_AGENT__H

#include <cmath>
#include <string>
#include <vector>
#include "enviro.h"

using namespace enviro;

class LeaderController : public Process, public AgentInterface {
    public:
    LeaderController() : Process(), AgentInterface(), paused(false), waypoint_index(0) {}

    void init() {
        watch("button_click", [&](Event& e) {
            if ( e.value()["name"] == "toggle_pause" ) {
                paused = !paused;
            }
        });
    }

    void start() {
        waypoint_index = 0;
        label("Leader", 18, 18);
    }

    void update() {
        center(x(), y());

        if ( paused ) {
            track_velocity(0, 0);
            label("Leader (paused)", 18, 18);
            emit(Event("leader_state", json{
                {"x", x()},
                {"y", y()},
                {"theta", angle()},
                {"paused", true}
            }));
            return;
        }

        const Point& target = waypoints[waypoint_index];
        double dx = target.x - x();
        double dy = target.y - y();
        double dist = std::sqrt(dx*dx + dy*dy);

        double front = sensor_value(0);
        double front_left = sensor_value(1);
        double front_right = sensor_value(2);

        if ( front < 22 || front_left < 16 || front_right < 16 ) {
            double omega = (front_left < front_right) ? -0.55 : 0.55;
            track_velocity(0, omega);
        } else {
            move_toward(target.x, target.y, 35, 110);
        }

        if ( dist < 25 ) {
            waypoint_index = (waypoint_index + 1) % waypoints.size();
        }

        decorate("<circle cx='0' cy='0' r='3' style='fill: #004d00'></circle>");
        label("Leader", 18, 18);

        emit(Event("leader_state", json{
            {"x", x()},
            {"y", y()},
            {"theta", angle()},
            {"paused", false}
        }));
    }

    void stop() {}

    private:
    struct Point {
        double x;
        double y;
    };

    bool paused;
    std::size_t waypoint_index;
    std::vector<Point> waypoints {
        {-250, -150},
        { 250, -150},
        { 250,  150},
        {-250,  150}
    };
};

class Leader : public Agent {
    public:
    Leader(json spec, World& world) : Agent(spec, world) {
        add_process(controller);
    }

    private:
    LeaderController controller;
};

DECLARE_INTERFACE(Leader)

#endif
