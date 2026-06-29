#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include "profile.h"
#include "utils.h"

void saveProfile(const Profile& profile) {
    system("mkdir save 2>nul");
    std::ofstream file("save/" + profile.name + ".txt");
    if (!file.is_open()) return;

    file << profile.name << "\n";
    file << profile.lastMissionUnlocked << "\n";
    file << profile.continueMission << "\n";
    file << profile.lastTimeRemaining << "\n";
    file << profile.logs.size() << "\n";

    for (const MissionLog& log : profile.logs) {
        file << log.missionNumber << "\n";
        file << log.missionName << "\n";
        file << log.passed << "\n";
        file << log.bestTimeRemaining << "\n";
    }
    file.close();
}

std::vector<Profile> loadAllProfiles() {
    std::vector<Profile> profiles;
    std::ifstream players("save/player.txt");
    if (!players.is_open()) return profiles;

    std::string name;
    while (std::getline(players, name)) {
        if (!name.empty()) {
            profiles.push_back(loadProfile(name));
        }
    }
    players.close();
    return profiles;
}

Profile loadProfile(const std::string& name) {
    Profile profile;
    profile.lastMissionUnlocked = 0;
    profile.continueMission     = 0;
    profile.lastTimeRemaining   = 0;

    std::ifstream file("save/" + name + ".txt");
    if (!file.is_open()) return profile;

    std::getline(file, profile.name);
    file >> profile.lastMissionUnlocked;
    file >> profile.continueMission;
    file >> profile.lastTimeRemaining;

    int logCount;
    file >> logCount;
    file.ignore();

    for (int i = 0; i < logCount; i++) {
        MissionLog log;
        file >> log.missionNumber;
        file.ignore();
        std::getline(file, log.missionName);
        file >> log.passed;
        file >> log.bestTimeRemaining;
        file.ignore();
        profile.logs.push_back(log);
    }
    file.close();
    return profile;
}

bool profileExists(const std::string& name) {
    std::ifstream file("save/" + name + ".txt");
    return file.is_open();
}

Profile selectProfile() {
    std::vector<Profile> profiles = loadAllProfiles();

    clearScreen();
    printSeparator();
    std::cout << "        LOAD PROFILE\n";
    printSeparator();

    if (profiles.empty()) {
        std::cout << "\n  No saved profiles found.\n";
        std::cout << "  Start a new game to create a profile.\n\n";
        printSeparator();
        std::cout << "  [0] Back to Main Menu\n";
        printSeparator();
        std::cout << "\n> ";
        std::string input;
        std::getline(std::cin, input);
        return Profile();
    }

    for (int i = 0; i < (int)profiles.size(); i++) {
        std::cout << "  [" << i + 1 << "] " << profiles[i].name << "\n";
    }
    std::cout << "\n  [0] Back to Main Menu\n";
    printSeparator();
    std::cout << "\n> ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 0 || choice > (int)profiles.size()) {
        return Profile();
    }
    return profiles[choice - 1];
}

void displayMissionLogs(const Profile& profile) {
    clearScreen();
    printSeparator();
    std::cout << "        MISSION LOGS\n";
    printSeparator();
    std::cout << "  Agent : " << profile.name << "\n\n";

    if (profile.logs.empty()) {
        std::cout << "  No missions completed yet.\n";
    } else {
        for (const MissionLog& log : profile.logs) {
            std::cout << "  CASE 00" << log.missionNumber
                      << " - \"" << log.missionName << "\"\n";
            std::cout << "  Status    : "
                      << (log.passed ? "PASSED" : "FAILED") << "\n";
            int minutes = log.bestTimeRemaining / 60;
            int seconds = log.bestTimeRemaining % 60;
            std::cout << "  Best Time : "
                      << (minutes < 10 ? "0" : "") << minutes << ":"
                      << (seconds < 10 ? "0" : "") << seconds
                      << " remaining\n";
            std::cout << "  ----------------------------------------\n";
        }
    }

    for (int i = profile.lastMissionUnlocked + 1; i <= 5; i++) {
        std::cout << "  CASE 00" << i << " - LOCKED\n";
    }

    printSeparator();
    std::cout << "  [0] Back\n";
    printSeparator();
    std::cout << "\n> ";
    std::string input;
    std::getline(std::cin, input);
}