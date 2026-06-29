#include <windows.h>
#include <conio.h>
#include <iostream>
#include <limits>
#include <chrono>
#include "utils.h"
#include "profile.h"
#include "mission.h"
#include "bomb.h"

void initBomb(Bomb& bomb, int missionNumber) {
    bomb.panelLocked = false;
    bomb.defused     = false;
    bomb.detonated   = false;
    bomb.timeRemaining = 180;

    if (missionNumber == 1) {
        bomb.pinAttempts  = 3;
        bomb.wireAttempts = 2;
        bomb.correctPin   = 1994;
        bomb.correctWire  = WIRE_YELLOW;
    } else if (missionNumber == 2) {
        bomb.pinAttempts  = 2;
        bomb.wireAttempts = 1;
        bomb.correctPin   = 1947;
        bomb.correctWire  = WIRE_BLUE;
    }
}

void timerThread(std::atomic<int>& timeLeft, std::atomic<bool>& active) {
    while (active && timeLeft > 0) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (active) {
            timeLeft--;
        }
    }
    if (timeLeft <= 0) {
        active = false;
    }
}

void displayBombPanel(const Bomb& bomb, int timeLeft, int missionNumber) {
    clearScreen();
    printSeparator();
    std::cout << "  BOMB PANEL - CASE 00" << missionNumber << "\n";
    printSeparator();

    int minutes = timeLeft / 60;
    int seconds = timeLeft % 60;
    std::cout << "  Timer  : ";
    setColor(COLOR_RED);
    std::cout << (minutes < 10 ? "0" : "") << minutes << ":"
              << (seconds < 10 ? "0" : "") << seconds << "\n";
    resetColor();

    std::cout << "  Wires  : ";
    setColor(COLOR_RED);    std::cout << "RED";
    resetColor();           std::cout << " | ";
    setColor(COLOR_BLUE);   std::cout << "BLUE";
    resetColor();           std::cout << " | ";
    setColor(COLOR_GREEN);  std::cout << "GREEN";
    resetColor();           std::cout << " | ";
    setColor(COLOR_YELLOW); std::cout << "YELLOW";
    resetColor();           std::cout << "\n";

    std::cout << "  Panel  : "
              << (bomb.panelLocked ? "LOCKED PERMANENTLY" : "LOCKED") << "\n";
    std::cout << "  Attempts remaining - PIN: "
              << (bomb.panelLocked ? 0 : bomb.pinAttempts)
              << " | Wire: " << bomb.wireAttempts << "\n";

    printSeparator();
    std::cout << "  [1] Investigate the scene\n";
    if (!bomb.panelLocked) {
        std::cout << "  [2] Enter panel code\n";
    }
    std::cout << "  [3] Cut a wire\n";
    std::cout << "  [4] Review briefing\n";
    printSeparator();
    std::cout << "\n> ";
}

void investigateScene(int missionNumber) {
    clearScreen();
    printSeparator();
    std::cout << "  INVESTIGATION - CASE 00" << missionNumber << "\n";
    printSeparator();
    std::cout << "\n  You scan the area around the bomb...\n\n";

    if (missionNumber == 1) {
        std::cout << "  A crumpled receipt is on the floor:\n\n";
        std::cout << "    =====================================\n";
        std::cout << "       HARTWELL ELECTRICAL SUPPLIES\n";
        std::cout << "    =====================================\n";
        std::cout << "      Wire Connectors x4       $3.50\n";
        std::cout << "      Insulation Tape          $2.17\n";
        std::cout << "      Timer Switch             $8.21\n";
        std::cout << "      Soldering Kit            $6.06\n";
        std::cout << "    -------------------------------------\n";
        std::cout << "      TOTAL                   $19.94\n";
        std::cout << "    =====================================\n\n";
        std::cout << "  Nothing else stands out.\n";

    } else if (missionNumber == 2) {
        std::cout << "  A station map is pinned to the wall.\n";
        std::cout << "  Three landmarks are drawn on it:\n\n";
        std::cout << "    - Purple Zone\n";
        std::cout << "    - Painting of the sky\n";
        std::cout << "    - White Waiting Lounge\n\n";
        std::cout << "  Fresh finger smudges run along the\n";
        std::cout << "  landmark at the top of the map.\n\n";
        std::cout << "  A damaged terminal sits beside the\n";
        std::cout << "  bomb. An IT report is still on screen:\n\n";
        std::cout << "    \"Terminal logs all attempts but only\n";
        std::cout << "     processes every third submission.\"\n\n";
        std::cout << "    Authentication Log:\n";
        std::cout << "    08:16 --- 4582\n";
        std::cout << "    08:18 --- 7319\n";
        std::cout << "    08:20 --- 1947\n\n";
        std::cout << "  Nothing else stands out.\n";
    }

    printSeparator();
    waitForEnter();
}

void handlePin(Bomb& bomb, std::atomic<int>& timeLeft, int missionNumber) {
    clearScreen();
    printSeparator();
    std::cout << "  ENTER PANEL CODE - CASE 00" << missionNumber << "\n";
    printSeparator();

    int minutes = timeLeft / 60;
    int seconds = timeLeft % 60;
    std::cout << "  Timer  : "
              << (minutes < 10 ? "0" : "") << minutes << ":"
              << (seconds < 10 ? "0" : "") << seconds << "\n";
    std::cout << "  PIN Attempts Remaining : " << bomb.pinAttempts << "\n\n";
    std::cout << "  Enter 4 digit code:\n> ";

    int pin;
    std::cin >> pin;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (pin == bomb.correctPin) {
        bomb.defused = true;
    } else {
        bomb.pinAttempts--;
        timeLeft -= 30;
        if (timeLeft < 0) timeLeft = 0;

        clearScreen();
        printSeparator();

        if (bomb.pinAttempts <= 0) {
            setColor(COLOR_RED);
            std::cout << "\n  INCORRECT. 30 seconds deducted.\n";
            std::cout << "  Panel is now permanently locked.\n";
            std::cout << "  Wire cutting is your only option now.\n";
            resetColor();
            bomb.panelLocked = true;
        } else {
            setColor(COLOR_RED);
            std::cout << "\n  INCORRECT. 30 seconds deducted.\n";
            std::cout << "  PIN Attempts Remaining : "
                      << bomb.pinAttempts << "\n";
            resetColor();
        }

        printSeparator();
        waitForEnter();
    }
}

void handleWire(Bomb& bomb, std::atomic<int>& timeLeft, int missionNumber) {
    clearScreen();
    printSeparator();
    std::cout << "  CUT A WIRE - CASE 00" << missionNumber << "\n";
    printSeparator();

    int minutes = timeLeft / 60;
    int seconds = timeLeft % 60;
    std::cout << "  Timer  : "
              << (minutes < 10 ? "0" : "") << minutes << ":"
              << (seconds < 10 ? "0" : "") << seconds << "\n";
    std::cout << "  Wire Attempts Remaining : " << bomb.wireAttempts << "\n\n";
    std::cout << "  Wires : ";
    setColor(COLOR_RED);    std::cout << "RED";
    resetColor();           std::cout << " | ";
    setColor(COLOR_BLUE);   std::cout << "BLUE";
    resetColor();           std::cout << " | ";
    setColor(COLOR_GREEN);  std::cout << "GREEN";
    resetColor();           std::cout << " | ";
    setColor(COLOR_YELLOW); std::cout << "YELLOW";
    resetColor();           std::cout << "\n\n";

    std::cout << "  Which wire do you cut?\n";
    std::cout << "  [1] RED\n";
    std::cout << "  [2] BLUE\n";
    std::cout << "  [3] GREEN\n";
    std::cout << "  [4] YELLOW\n";
    printSeparator();
    std::cout << "\n> ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    WireColor chosen = static_cast<WireColor>(choice - 1);

    if (chosen == bomb.correctWire) {
        bomb.defused = true;
    } else {
        bomb.wireAttempts--;
        timeLeft -= 40;
        if (timeLeft < 0) timeLeft = 0;

        clearScreen();
        printSeparator();

        if (bomb.wireAttempts <= 0) {
            bomb.detonated = true;
        } else {
            setColor(COLOR_RED);
            std::cout << "\n  WRONG WIRE. 40 seconds deducted.\n";
            std::cout << "  Wire Attempts Remaining : "
                      << bomb.wireAttempts << "\n";
            std::cout << "  Be careful. One attempt left.\n";
            resetColor();
            printSeparator();
            waitForEnter();
        }
    }
}

bool runBomb(Mission& mission, Profile& profile) {
    Bomb bomb;
    initBomb(bomb, mission.missionNumber);

    std::atomic<int>  timeLeft(bomb.timeRemaining);
    std::atomic<bool> active(true);

    std::thread timer(timerThread, std::ref(timeLeft), std::ref(active));

    while (!bomb.defused && !bomb.detonated) {
        if (timeLeft <= 0) {
            bomb.detonated = true;
            break;
        }

        displayBombPanel(bomb, timeLeft, mission.missionNumber);

        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            investigateScene(mission.missionNumber);
        } else if (choice == 2 && !bomb.panelLocked) {
            handlePin(bomb, timeLeft, mission.missionNumber);
        } else if (choice == 3) {
            handleWire(bomb, timeLeft, mission.missionNumber);
        } else if (choice == 4) {
            displayBriefing(mission);
        }
    }

    active = false;
    timer.join();

    if (bomb.defused) {
        profile.lastTimeRemaining = timeLeft;

        clearScreen();
        printSeparator();
        setColor(COLOR_GREEN);
        std::cout << "\n  Congratulations! The bomb has been defused.\n";
        std::cout << "  CASE 00" << mission.missionNumber << " CLOSED.\n\n";
        printSeparator();
        resetColor();
        waitForEnter();
        return true;

    } else {
        clearScreen();
        printSeparator();
        setColor(COLOR_RED);

        if (timeLeft <= 0) {
            std::cout << "\n  TIME IS UP.\n";
        } else {
            std::cout << "\n  WRONG WIRE.\n";
        }

        std::cout << "  ...\n  DETONATION.\n\n";
        printSeparator();
        std::cout << "\n  MISSION FAILED.\n\n";
        printSeparator();
        resetColor();
        std::cout << "\n  Try again? [Y / N]\n> ";

        char retry;
        std::cin >> retry;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (retry == 'Y' || retry == 'y') {
            return runBomb(mission, profile);
        }

        return false;
    }
}