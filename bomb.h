#pragma once
#include <string>
#include <atomic>
#include <thread>
#include "profile.h"
#include "mission.h"

enum WireColor {
    WIRE_RED    = 0,
    WIRE_BLUE   = 1,
    WIRE_GREEN  = 2,
    WIRE_YELLOW = 3
};

struct Bomb {
    int timeRemaining;
    int pinAttempts;
    int wireAttempts;
    bool panelLocked;
    bool defused;
    bool detonated;
    int correctPin;
    WireColor correctWire;
};

void initBomb(Bomb& bomb, int missionNumber);
bool runBomb(Mission& mission, Profile& profile);
void displayBombPanel(const Bomb& bomb, int timeLeft, int missionNumber);
void investigateScene(int missionNumber);
void handlePin(Bomb& bomb, std::atomic<int>& timeLeft, int missionNumber);
void handleWire(Bomb& bomb, std::atomic<int>& timeLeft, int missionNumber);
void timerThread(std::atomic<int>& timeLeft, std::atomic<bool>& active);