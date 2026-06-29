#include <windows.h>
#include <fstream>
#include <iostream>
#include <limits>
#include "utils.h"
#include "profile.h"
#include "mission.h"
#include "bomb.h"

Mission getMission(int missionNumber) {
    Mission mission;
    mission.missionNumber = 0;
    mission.missionName   = "";
    mission.location      = "";
    mission.briefing      = "";

    if (missionNumber == 1) {
        mission.missionNumber = 1;
        mission.missionName   = "THE WAREHOUSE";
        mission.location      = "Dock 7";
        mission.briefing      =
            "Bomb reported at Dock 7.\n\n"
            "Officers on the scene have gathered\n"
            "the following on the suspect:\n\n"
            "  - Former electrician\n"
            "  - Known to be obsessive and calculated\n"
            "  - Interrogated twice, never charged\n"
            "  - Officers noted he repeated one thing\n"
            "    in both interrogations:\n"
            "    \"I always use what I spend.\"\n"
            "    They never understood what he meant.\n"
            "  - A colleague confirmed he changed his\n"
            "    locker color to yellow while working\n"
            "    there\n\n"
            "You have 3 minutes. Figure it out.";
    }

    if (missionNumber == 2) {
        mission.missionNumber = 2;
        mission.missionName   = "CENTRAL TRAIN STATION";
        mission.location      = "Platform 4";
        mission.briefing      =
            "Bomb threat confirmed at Central Train\n"
            "Station. Device found inside a restricted\n"
            "service corridor beneath Platform 4.\n"
            "Too unstable to move. Must be defused\n"
            "on site.\n\n"
            "Officers on the scene have gathered\n"
            "the following on the suspect:\n\n"
            "  - Entered through a restricted\n"
            "    maintenance entrance using a\n"
            "    temporary staff pass that had\n"
            "    already been clocked out\n\n"
            "  - Security believes he knew the\n"
            "    station layout well\n\n"
            "  - CCTV shows him standing at the\n"
            "    station map for nearly a minute\n"
            "    before moving — fingers only\n"
            "    touched one drawing, located at\n"
            "    the top of the map\n\n"
            "  - IT Security flagged unusual\n"
            "    authentication activity on the\n"
            "    maintenance terminal shortly\n"
            "    before the device was planted\n\n"
            "You have 3 minutes. Figure it out.";
    }

    return mission;
}

void displayBriefing(const Mission& mission) {
    clearScreen();
    printSeparator();
    std::cout << "  CASE 00" << mission.missionNumber
              << " - " << mission.missionName << "\n";
    printSeparator();
    std::cout << "\n" << mission.briefing << "\n\n";
    printSeparator();
    waitForEnter();
}

bool startMission(Mission& mission, Profile& profile) {
    clearScreen();
    printSeparator();
    std::cout << "\n  Starting mission...\n\n";
    printSeparator();
    Sleep(3000);

    displayBriefing(mission);
    bool passed = runBomb(mission, profile);
    return passed;
}

void askToSave(Profile& profile) {
    clearScreen();
    printSeparator();
    std::cout << "        SAVE GAME\n";
    printSeparator();
    std::cout << "\n  Would you like to save your progress?\n\n";
    std::cout << "  [1] Yes\n";
    std::cout << "  [2] No\n";
    printSeparator();
    std::cout << "\n> ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 1) {
        system("mkdir save 2>nul");
        saveProfile(profile);

        bool found = false;
        std::ifstream checkFile("save/player.txt");
        std::string line;
        while (std::getline(checkFile, line)) {
            if (line == profile.name) {
                found = true;
                break;
            }
        }
        checkFile.close();

        if (!found) {
            std::ofstream playerFile("save/player.txt", std::ios::app);
            playerFile << profile.name << "\n";
            playerFile.close();
        }

        clearScreen();
        printSeparator();
        setColor(COLOR_GREEN);
        std::cout << "\n  Progress saved successfully.\n";
        std::cout << "  Well done, " << profile.name << ".\n\n";
        resetColor();
        printSeparator();
        waitForEnter();
    }
}

void displayUnitMenuNew(Profile& profile) {
    while (true) {
        clearScreen();
        printSeparator();
        std::cout << "        BOMB DISPOSAL UNIT\n";
        printSeparator();
        std::cout << "  Agent : " << profile.name << "\n\n";
        std::cout << "  [1] Start Mission (Case 001)\n";
        std::cout << "  [2] Select Mission\n";
        std::cout << "  [3] Mission Logs\n";
        std::cout << "  [4] Exit to Main Menu\n";
        printSeparator();
        std::cout << "\n> ";

        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            Mission mission = getMission(1);
            bool passed = startMission(mission, profile);

            if (passed) {
                profile.continueMission     = 2;
                profile.lastMissionUnlocked = 1;

                bool logFound = false;
                for (MissionLog& log : profile.logs) {
                    if (log.missionNumber == 1) {
                        if (profile.lastTimeRemaining > log.bestTimeRemaining) {
                            log.bestTimeRemaining = profile.lastTimeRemaining;
                        }
                        logFound = true;
                        break;
                    }
                }

                if (!logFound) {
                    MissionLog log;
                    log.missionNumber     = 1;
                    log.missionName       = mission.missionName;
                    log.passed            = true;
                    log.bestTimeRemaining = profile.lastTimeRemaining;
                    profile.logs.push_back(log);
                }

                askToSave(profile);
                displayUnitMenuReturning(profile);
                return;
            }

        } else if (choice == 2) {
            displayMissionSelect(profile);
        } else if (choice == 3) {
            displayMissionLogs(profile);
        } else if (choice == 4) {
            return;
        }
    }
}

void displayUnitMenuReturning(Profile& profile) {
    while (true) {
        clearScreen();
        printSeparator();
        std::cout << "        BOMB DISPOSAL UNIT\n";
        printSeparator();
        std::cout << "  Agent : " << profile.name << "\n\n";

        if (profile.continueMission == 0) {
            std::cout << "  [1] No Active Mission\n";
        } else {
            std::cout << "  [1] Continue Mission (Case 00"
                      << profile.continueMission << ")\n";
        }

        std::cout << "  [2] Select Mission\n";
        std::cout << "  [3] Mission Logs\n";
        std::cout << "  [4] Exit to Main Menu\n";
        printSeparator();
        std::cout << "\n> ";

        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            if (profile.continueMission == 0) {
                clearScreen();
                printSeparator();
                std::cout << "\n  No active mission found.\n";
                std::cout << "  Head to Select Mission to get started.\n\n";
                printSeparator();
                waitForEnter();
            } else {
                Mission mission = getMission(profile.continueMission);
                bool passed = startMission(mission, profile);

                if (passed) {
                    int completedMission        = profile.continueMission;
                    profile.continueMission     = completedMission + 1;
                    profile.lastMissionUnlocked = completedMission;

                    bool logFound = false;
                    for (MissionLog& log : profile.logs) {
                        if (log.missionNumber == completedMission) {
                            if (profile.lastTimeRemaining > log.bestTimeRemaining) {
                                log.bestTimeRemaining = profile.lastTimeRemaining;
                            }
                            logFound = true;
                            break;
                        }
                    }

                    if (!logFound) {
                        MissionLog log;
                        log.missionNumber     = completedMission;
                        log.missionName       = mission.missionName;
                        log.passed            = true;
                        log.bestTimeRemaining = profile.lastTimeRemaining;
                        profile.logs.push_back(log);
                    }

                    askToSave(profile);
                }
            }

        } else if (choice == 2) {
            displayMissionSelect(profile);
        } else if (choice == 3) {
            displayMissionLogs(profile);
        } else if (choice == 4) {
            return;
        }
    }
}

void displayMissionSelect(Profile& profile) {
    clearScreen();
    printSeparator();
    std::cout << "        SELECT MISSION\n";
    printSeparator();
    std::cout << "  Agent : " << profile.name << "\n\n";

    for (int i = 1; i <= 5; i++) {
        if (i <= profile.lastMissionUnlocked + 1) {
            std::string status = (i <= profile.lastMissionUnlocked)
                                 ? "PASSED" : "NEW";
            Mission m = getMission(i);
            std::cout << "  [" << i << "] CASE 00" << i
                      << " - " << m.missionName
                      << "    " << status << "\n";
        } else {
            std::cout << "  [" << i << "] CASE 00" << i
                      << " -              LOCKED\n";
        }
    }

    std::cout << "\n  [0] Back\n";
    printSeparator();
    std::cout << "\n> ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 0 || choice > 5) return;

    if (choice > profile.lastMissionUnlocked + 1) {
        clearScreen();
        printSeparator();
        std::cout << "\n  This mission is locked.\n";
        std::cout << "  Complete previous missions to unlock.\n\n";
        printSeparator();
        waitForEnter();
        return;
    }

    Mission mission = getMission(choice);
    bool passed = startMission(mission, profile);

    if (passed) {
        int completedMission        = choice;
        profile.continueMission     = completedMission + 1;
        profile.lastMissionUnlocked = completedMission;

        bool logFound = false;
        for (MissionLog& log : profile.logs) {
            if (log.missionNumber == completedMission) {
                if (profile.lastTimeRemaining > log.bestTimeRemaining) {
                    log.bestTimeRemaining = profile.lastTimeRemaining;
                }
                logFound = true;
                break;
            }
        }

        if (!logFound) {
            MissionLog log;
            log.missionNumber     = completedMission;
            log.missionName       = mission.missionName;
            log.passed            = true;
            log.bestTimeRemaining = profile.lastTimeRemaining;
            profile.logs.push_back(log);
        }

        askToSave(profile);
    }
}