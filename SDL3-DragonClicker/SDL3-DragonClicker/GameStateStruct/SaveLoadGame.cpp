#include "Headers/includeFunctions.hpp"
#include "Headers/global.hpp"

void loadGameState(GameState& gameState)
{
    std::ifstream file("savegame.json");
    if (!file)
    {
        std::cout << "Brak pliku zapisu! Tworzenie nowej gry." << std::endl;
        createDragons(); // Tworzenie domyœlnych smoków
        createButtons(); // Tworzenie domyœlnych przycisków
        createDragonButtons(); // Tworzenie domyœlnych przycisków smoka
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
    int dragonButtonIndex = 0;
    for (const auto& upgradeData : jsonData["dragonUpgrades"])
    {
        float loadedDragonUpgradeCost = upgradeData.value("cost", 10.0f);
        long double loadedMultiplier = upgradeData.value("multiplier", 1.5);

        std::cout << "£adowanie DragonUpgradeButton [" << dragonButtonIndex << "] - Koszt: " << loadedDragonUpgradeCost << ", Mno¿nik: " << loadedMultiplier << std::endl; // Log ³adowania DragonUpgradeButton
        gameState.dragonButtons.emplace_back(
            static_cast<long int>(loadedDragonUpgradeCost),
            loadedMultiplier
        );
        dragonButtonIndex++;
    }
    std::cout << "Za³adowano " << gameState.dragonButtons.size() << " DragonUpgradeButtons." << std::endl;

    gameState.buttons.clear();
    int buttonIndex = 0;
    for (const auto& buttonData : jsonData["buttons"])
    {
        float loadedUpgradeCost = buttonData.value("upgradeCost", 50.0f);
        float loadedIncrementValue = buttonData.value("incrementValue", 0.0f);
        bool loadedIsActive = buttonData.value("isActive", false);

        std::cout << "£adowanie UpgradeButton [" << buttonIndex << "] - Koszt ulepszenia (z JSON): " << loadedUpgradeCost << ", Increment value: " << loadedIncrementValue << ", Aktywny: " << loadedIsActive << std::endl; // LOG 1: Wypisujemy wartoœæ za³adowan¹ z JSON

        float incrementValueChangeValues[] = { 0.1f, 0.5f, 1.0f };
        float incrementValueChangeToUse = 0.1f;
        if (buttonIndex < sizeof(incrementValueChangeValues) / sizeof(incrementValueChangeValues[0]))
        {
            incrementValueChangeToUse = incrementValueChangeValues[buttonIndex];
        }

        gameState.buttons.emplace_back(
            loadedUpgradeCost,
            loadedIncrementValue,
            incrementValueChangeToUse
        );

        std::cout << "£adowanie UpgradeButton [" << buttonIndex << "] - Koszt ulepszenia (po utworzeniu obiektu): " << gameState.buttons.back().upgradeCost << std::endl; // LOG 2: Wypisujemy cenê obiektu button po utworzeniu

        if (loadedIsActive)
        {
            gameState.buttons.back().activate();
        }
        buttonIndex++;
    }

    std::cout << "Za³adowano " << gameState.buttons.size() << " UpgradeButtons." << std::endl;
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

    jsonData["dragonButtons"] = nlohmann::json::array();
    for (const auto& upgrade : gameState.dragonButtons)
    {
        std::cout << "Zapisywanie DragonUpgradeButton - Koszt: " << upgrade.getDragonUpgradeCost() << ", Mno¿nik: " << upgrade.getMultiplier() << std::endl; // Log zapisywania DragonUpgradeButton
        jsonData["dragonUpgrades"].push_back({
            {"multiplier", upgrade.getMultiplier()},
            {"cost", upgrade.getDragonUpgradeCost()}
            });
    }

    jsonData["buttons"] = nlohmann::json::array();
    for (const auto& button : gameState.buttons)
    {
        std::cout << "Zapisywanie UpgradeButton - Koszt ulepszenia (przed zapisem JSON): " << button.getUpgradeCost() << ", Increment value: " << button.getIncrementValue() << ", Aktywny: " << button.isActive() << std::endl; // LOG 3: Wypisujemy cenê przed zapisem do JSON
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