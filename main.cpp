#include <windows.h>
#include <iostream>
#include <limits>
#include <fstream>
#include "utils.h"
#include "profile.h"
#include "mission.h"
#include "bomb.h"

bool hasUnsavedProgress(const Profile& profile) {
    if (profile.name.empty()) return false;
    if (profile.logs.empty()) return false;
    if (!profileExists(profile.name)) return true;
    Profile saved = loadProfile(profile.name);
    return saved.lastMissionUnlocked != profile.lastMissionUnlocked;
}

void displayMainMenu() {
    clearScreen();
    printSeparator();
    std::cout << "        WELCOME TO BOMB DEFUSAL GAME\n";
    printSeparator();
    std::cout << "\n";
    std::cout << "  [1] New Game\n";
    std::cout << "  [2] Load Profile\n";
    std::cout << "  [3] Check Game Info\n";
    std::cout << "  [4] Quit Game\n";
    printSeparator();
    std::cout << "\n> ";
}

void displayGameInfo() {
    clearScreen();
    printSeparator();
    std::cout << "        DEFUSE - GAME INFO\n";
    printSeparator();
    std::cout << "\n";
    std::cout << "  You are a bomb disposal technician\n";
    std::cout << "  working for an elite unit tasked with\n";
    std::cout << "  neutralizing explosive threats across\n";
    std::cout << "  the city.\n\n";
    std::cout << "  Every case is different. Every bomb\n";
    std::cout << "  tells a story. Your job is not just to\n";
    std::cout << "  cut wires - it is to think, investigate,\n";
    std::cout << "  and understand the mind of the person\n";
    std::cout << "  who built it.\n\n";
    std::cout << "  Each mission drops you into a new\n";
    std::cout << "  incident. A new location. A new suspect.\n";
    std::cout << "  A new bomb. The clues are always there -\n";
    std::cout << "  left behind accidentally or hidden in\n";
    std::cout << "  plain sight. Find them. Use them.\n\n";
    std::cout << "  The stakes are real. The timer never\n";
    std::cout << "  stops. One wrong move costs you time.\n";
    std::cout << "  Two wrong moves could cost everything.\n\n";
    std::cout << "  You are not just defusing bombs.\n";
    std::cout << "  You are solving cases.\n\n";
    std::cout << "  Good luck, technician. The city is\n";
    std::cout << "  counting on you.\n\n";
    printSeparator();
    std::cout << "  [0] Return to Main Menu\n";
    printSeparator();
    std::cout << "\n> ";
    std::string input;
    std::getline(std::cin, input);
}

void newGame() {
    clearScreen();
    printSeparator();
    std::cout << "        NEW GAME\n";
    printSeparator();
    std::cout << "\n  Enter your name, technician:\n> ";

    std::string name;
    std::getline(std::cin, name);

    Profile profile;
    profile.name                = name;
    profile.lastMissionUnlocked = 0;
    profile.continueMission     = 0;
    profile.lastTimeRemaining   = 0;

    clearScreen();
    printSeparator();
    setColor(COLOR_GREEN);
    std::cout << "\n  WELCOME, " << name << "\n\n";
    resetColor();
    std::cout << "  Your profile has been created.\n";
    std::cout << "  Report to your first assignment.\n\n";
    printSeparator();
    waitForEnter();

    displayUnitMenuNew(profile);
}

void loadGame() {
    Profile profile = selectProfile();
    if (profile.name.empty()) return;

    clearScreen();
    printSeparator();
    setColor(COLOR_GREEN);
    std::cout << "\n  WELCOME BACK, " << profile.name << "\n\n";
    resetColor();
    std::cout << "  Resuming your progress...\n\n";
    printSeparator();
    waitForEnter();

    displayUnitMenuReturning(profile);
}

void quitGame(const Profile& profile) {
    if (hasUnsavedProgress(profile)) {
        clearScreen();
        printSeparator();
        std::cout << "        UNSAVED PROGRESS\n";
        printSeparator();
        std::cout << "\n  You have unsaved progress.\n";
        std::cout << "  Would you like to save before quitting?\n\n";
        std::cout << "  [1] Save and quit\n";
        std::cout << "  [2] Quit without saving\n";
        std::cout << "  [3] Go back\n";
        printSeparator();
        std::cout << "\n> ";

        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            Profile p = profile;
            askToSave(p);
            clearScreen();
            printSeparator();
            std::cout << "\n  See you next time, "
                      << profile.name << ".\n\n";
            printSeparator();
            exit(0);
        } else if (choice == 2) {
            clearScreen();
            printSeparator();
            std::cout << "\n  See you next time, "
                      << profile.name << ".\n\n";
            printSeparator();
            exit(0);
        }
        return;
    }

    clearScreen();
    printSeparator();
    std::cout << "        QUIT GAME\n";
    printSeparator();
    std::cout << "\n  Are you sure you want to quit?\n\n";
    std::cout << "  [1] Yes, quit\n";
    std::cout << "  [2] No, go back\n";
    printSeparator();
    std::cout << "\n> ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 1) {
        clearScreen();
        printSeparator();
        if (!profile.name.empty()) {
            std::cout << "\n  See you next time, "
                      << profile.name << ".\n\n";
        } else {
            std::cout << "\n  See you next time.\n\n";
        }
        printSeparator();
        exit(0);
    }
}

int main() {
    clearScreen();
    printSeparator();
    std::cout << "\n";
    std::cout << "    BOMB DISPOSAL GAME\n";
    std::cout << "\n";
    printSeparator();
    std::cout << "\n    Press [ENTER] to start...\n";
    std::cin.get();

    Profile currentProfile;
    currentProfile.lastMissionUnlocked = 0;
    currentProfile.continueMission     = 0;
    currentProfile.lastTimeRemaining   = 0;

    while (true) {
        displayMainMenu();

        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            newGame();
        } else if (choice == 2) {
            loadGame();
        } else if (choice == 3) {
            displayGameInfo();
        } else if (choice == 4) {
            quitGame(currentProfile);
        }
    }

    return 0;
}
