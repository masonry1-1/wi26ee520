# Leader Follower Train (Enviro Final Project)

This project is an Enviro / Elma simulation of a leader robot driving a looping route while a train of follower robots tries to maintain spacing behind it. The project is designed to satisfy the course final-project requirement to use Enviro in a substantial way.

## What the project does

- One **leader** robot drives around a rectangular path.
- Multiple **followers** trail behind the leader using the leader's broadcast position and heading.
- Followers use **range sensors** to reduce collisions with walls and other robots.
- The arena includes **internal obstacles**, so the train has to adjust as it moves.
- A **button** lets you spawn more followers during runtime.
- Another button lets you **pause / resume** the leader.

This gives you:
- multi-agent behavior
- event-based communication
- sensor-based navigation
- a simulation that is easy to demo live

## Project structure

```text
enviro_leader_follower_project/
├── Makefile
├── config.json
├── README.md
├── LICENSE
├── defs/
│   ├── coordinator.json
│   ├── follower.json
│   └── leader.json
├── lib/
└── src/
    ├── Makefile
    ├── coordinator.cc
    ├── coordinator.h
    ├── follower.cc
    ├── follower.h
    ├── leader.cc
    ├── leader.h
