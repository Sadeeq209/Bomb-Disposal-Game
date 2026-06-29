#pragma once
#include <string>
#include "profile.h"

struct Mission {
    int missionNumber;
    std::string missionName;
    std::string location;
    std::string briefing;
};

Mission getMission(int missionNumber);
void displayBriefing(const Mission& mission);
bool startMission(Mission& mission, Profile& profile);
void displayUnitMenuNew(Profile& profile);
void displayUnitMenuReturning(Profile& profile);
void displayMissionSelect(Profile& profile);
void askToSave(Profile& profile);