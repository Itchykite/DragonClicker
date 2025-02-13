#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include <string>
#include "Dragon.hpp"
#include "DragonUpgrades.hpp"
#include "UpgradeButton.hpp"

struct GameState
{
    long double incrementScore = 1.0;
    long double score = 0.0;
    long double dragonCoins = 0.0;
    std::string s_score;
    std::string lastSaveTime;

    std::vector<Dragon> dragons = 
    {
        {10.0, 100, 100}
    };

    std::vector<DragonUpgrades> dragonButtons = 
    {
        {10, 1.5},
        {30, 2.5}
    };

    std::vector<UpgradeButton> buttons = 
    {
        {50.0, 0.0, 0.1f},
        {100.0, 0.0, 0.1f},
        {200.0, 0.0, 0.1f},
        {500.0, 0.0, 0.1f}
    };
};

void saveGameState(const GameState& gameState);
void loadGameState(GameState& gameState);

std::string getCurrentDateTime();
time_t parseDateTime(const std::string& dateTimeStr);