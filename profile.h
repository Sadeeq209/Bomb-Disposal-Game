#pragma once
#include <string>
#include <vector>

struct MissionLog {
    int missionNumber;
    std::string missionName;
    bool passed;
    int bestTimeRemaining;
};

struct Profile {
    std::string name;
    int lastMissionUnlocked;
    int continueMission;
    int lastTimeRemaining;
    std::vector<MissionLog> logs;
};

void saveProfile(const Profile& profile);
std::vector<Profile> loadAllProfiles();
Profile loadProfile(const std::string& name);
bool profileExists(const std::string& name);
Profile selectProfile();
void displayMissionLogs(const Profile& profile);