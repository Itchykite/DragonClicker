#pragma once
#include <vector>
#include <string>
#include "DragonClass.hpp"
#include "DragonUpgradeClass.hpp"
#include "UpgradeButtonClass.hpp"

struct GameState
{
    long double incrementScore = { 1.0f };
    long double score = { 0.0f };
    long double dragonCoins = { 0.0f };
    std::string s_score = std::to_string(score);
    std::string lastSaveTime;

    std::vector<Dragon> dragons{};
    std::vector<DragonUpgrades> dragonButtons{};
    std::vector<UpgradeButton> buttons{};
};