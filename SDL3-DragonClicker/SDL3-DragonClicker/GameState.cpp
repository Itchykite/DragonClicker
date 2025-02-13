#include "GameState.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <chrono>

std::string getCurrentDateTime()
{
    auto now = std::chrono::system_clock::now();
    std::time_t in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%FT%T%z");

    return ss.str();
}

time_t parseDateTime(const std::string& dateTimeStr)
{
    std::tm t{};
    std::istringstream ss(dateTimeStr);

    ss >> std::get_time(&t, "%FT%T%z");
    if (ss.fail())
    {
        throw std::runtime_error("Err");
    }

    return mktime(&t);
}

void saveGameState(const GameState& gameState)
{
    nlohmann::json jsonData;

    jsonData["score"] = gameState.score;
    jsonData["incrementScore"] = gameState.incrementScore;
    jsonData["dragonCoins"] = gameState.dragonCoins;
    jsonData["lastSaveTime"] = getCurrentDateTime();

    jsonData["dragons"] = nlohmann::json::array();
    for (const auto& dragon : gameState.dragons)
    {
        jsonData["dragons"].push_back({
            {"health", dragon.health},
            {"baseHealth", dragon.baseHealth},
            {"dragonSlayedPrice", dragon.dragonSlayedPrice}
            });
    }

    jsonData["dragonUpgrades"] = nlohmann::json::array();
    for (const auto& upgrade : gameState.dragonButtons)
    {
        jsonData["dragonUpgrades"].push_back({
            {"multiplier", upgrade.getMultiplier()},
            {"cost", upgrade.getDragonUpgradeCost()}
            });
    }

    jsonData["buttons"] = nlohmann::json::array();
    for (const auto& button : gameState.buttons)
    {
        jsonData["buttons"].push_back({
            {"incrementValue", button.getIncrementValue()},
            {"upgradeCost", button.getUpgradeCost()},
            {"isActive", button.isActive()}
            });
    }

    std::ofstream file("savegame.json");
    if (file)
    {
        file << std::setw(4) << jsonData << std::endl;
    }
}

void loadGameState(GameState& gameState)
{
    std::ifstream file("savegame.json");
    if (!file.is_open())
    {
        std::cerr << "Plik savegame.json nie istnieje. Tworzenie nowego pliku z domyœlnymi wartoœciami." << std::endl;
        gameState = GameState();
        saveGameState(gameState);
        return;
    }

    nlohmann::json jsonData;
    file >> jsonData;

    gameState.score = jsonData.value("score", 0.0);
    gameState.incrementScore = jsonData.value("incrementScore", 1.0);
    gameState.dragonCoins = jsonData.value("dragonCoins", 0.0);
    gameState.lastSaveTime = jsonData.value("lastSaveTime", "");

    gameState.dragons.clear();
    for (const auto& dragonData : jsonData["dragons"])
    {
        Dragon dragon(dragonData["dragonSlayedPrice"].get<double>());
        dragon.health = dragonData["health"].get<long double>();
        dragon.baseHealth = dragonData["baseHealth"].get<long double>();
        gameState.dragons.push_back(dragon);
    }

    gameState.dragonButtons.clear();
    for (const auto& upgradeData : jsonData["dragonUpgrades"])
    {
        gameState.dragonButtons.emplace_back(
            upgradeData["cost"].get<long int>(),
            upgradeData["multiplier"].get<long double>()
        );
    }

    gameState.buttons.clear();
    for (const auto& buttonData : jsonData["buttons"])
    {
        gameState.buttons.emplace_back(
            buttonData["upgradeCost"].get<float>(),
            buttonData["incrementValue"].get<float>(),
            0.1f
        );

        if (buttonData["isActive"].get<bool>())
        {
            gameState.buttons.back().activate();
        }
    }
}