/* clear.c ... */

/*
 * This example code creates an SDL window and renderer, and then clears the
 * window to a different color every frame, so you'll effectively get a window
 * that's smoothly fading between colors.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */

#define _CRT_SECURE_NO_WARNINGS
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <nlohmann/json.hpp>

#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <ctime>
#include <chrono>

 // Rozmiar okna
static int WINDOW_WIDTH = 1920;
static int WINDOW_HEIGHT = 1080;

/* We will use this renderer to draw into this window every frame. */
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* texture = NULL;
static SDL_Texture* incrementValueTexture = NULL;
static SDL_Texture* healthValueTexture = NULL;
static SDL_Texture* dragonCoinTexture = NULL;
static TTF_Font* font = NULL;
static TTF_Font* incrementValueFont = NULL;
static TTF_Font* HPValueFont = NULL;
static TTF_Font* upgradeValueFont = NULL;

static SDL_Texture* backgroundImage = nullptr;
static SDL_Texture* dragon1 = nullptr;
static SDL_Texture* dragonCoin = nullptr;

Uint32 lastIncrementTime = 0;
Uint32 lastSaveTime = SDL_GetTicks();
const Uint32 autoSaveInterval = 60000;

class Dragon
{
public:
    long double health;
    long double baseHealth = 100;
    double dragonSlayedPrice = 0;

    Dragon(double dragonSlayedPrice) : dragonSlayedPrice(dragonSlayedPrice)
    {
        health = baseHealth;
        float clickFieldSizePercentage = 0.75f;
        float squareSize = std::min(WINDOW_WIDTH, WINDOW_HEIGHT) * clickFieldSizePercentage;
        clickField = { (WINDOW_WIDTH - squareSize) / 2.0f, (WINDOW_HEIGHT - squareSize) / 2.0f, squareSize, squareSize };
    }

    Dragon(double dragonSlayedPrice, long double health, long double baseHealth)
        : dragonSlayedPrice(dragonSlayedPrice), health(health), baseHealth(baseHealth)
    {
        if (health == 0.0) health = baseHealth;
    }
    void render(SDL_Renderer* renderer)
    {
        float dragonWidth, dragonHeight;
        SDL_GetTextureSize(dragon1, &dragonWidth, &dragonHeight);

        float scale = 1.2f;
        float newDragonWidth = static_cast<float>(dragonWidth * scale);
        float newDragonHeight = static_cast<float>(dragonHeight * scale);

        float posX = (WINDOW_WIDTH - newDragonWidth) / 2.0f;
        float posY = (WINDOW_HEIGHT - newDragonHeight) / 2.0f;

        SDL_FRect dragonField = { posX, posY, static_cast<float>(newDragonWidth), static_cast<float>(newDragonHeight) };
        SDL_RenderTexture(renderer, dragon1, NULL, &dragonField);
    }

    float getX() const { return clickField.x; }
    float getY() const { return clickField.y; }
    float getW() const { return clickField.w; }
    float getH() const { return clickField.h; }

private:
    SDL_FRect clickField;
};

class DragonUpgrades
{
public:
    SDL_Texture* texture = nullptr;
    SDL_FRect textRect{};

    long double multiplier{};
    long int dragonUpgradeCost{};

    DragonUpgrades(long int dragonUpgradeCost, long double multiplier) : dragonUpgradeCost(dragonUpgradeCost), multiplier(multiplier)
    {
        float clickFieldSizePercentage = 0.25f;
        float squareSize = static_cast<float>(std::min(WINDOW_WIDTH, WINDOW_HEIGHT) * clickFieldSizePercentage);
        int margin = 20;
        float rectangleHeight = squareSize / 4;

        // Automatyczne pozycjonowanie przycisków jeden pod drugim
        upgradeButtonField =
        {
            static_cast<float>(margin), // Lewa krawêdŸ z marginesem
            static_cast<float>(yOffset),
            squareSize,
            rectangleHeight
        };

        // Zwiêkszenie pozycji dla kolejnego przycisku
        yOffset += rectangleHeight + 10;
    }

    ~DragonUpgrades()
    {
        if (texture)
        {
            SDL_DestroyTexture(texture);
        }
    }

    void render(SDL_Renderer* renderer)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &upgradeButtonField);

        if (texture)
        {
            textRect.x = upgradeButtonField.x + (upgradeButtonField.w - textRect.w) / 2;
            textRect.y = upgradeButtonField.y + (upgradeButtonField.h - textRect.h) / 2;

            SDL_RenderTexture(renderer, texture, nullptr, &textRect);
        }
    }

    float getX() const { return upgradeButtonField.x; }
    float getY() const { return upgradeButtonField.y; }
    float getW() const { return upgradeButtonField.w; }
    float getH() const { return upgradeButtonField.h; }
    long double getMultiplier() const { return multiplier; }
    long int getDragonUpgradeCost() const { return dragonUpgradeCost; }

private:
    static float yOffset; // Przechowuje przesuniêcie w osi Y dla kolejnych przycisków
    SDL_FRect upgradeButtonField{};
};

float DragonUpgrades::yOffset = 100.0f;

class UpgradeButton
{
public:
    SDL_Texture* texture = nullptr;
    SDL_FRect textRect{};

    bool isObjectUpgrading = false;
    float incrementValue;
    float upgradeCost;
    static Uint32 lastUpdateTime;

    UpgradeButton(float upgradeCost, float incrementValue, float incrementValueChange)
        : upgradeCost(upgradeCost), incrementValue(incrementValue), incrementValueChange(incrementValueChange), isObjectUpgrading(false)
    {
        float clickFieldSizePercentage = 0.25f;
        float squareSize = static_cast<float>(std::min(WINDOW_WIDTH, WINDOW_HEIGHT) * clickFieldSizePercentage);
        int margin = 20;
        float rectangleHeight = squareSize / 4;

        // Automatyczne pozycjonowanie przycisków jeden pod drugim
        upgradeButtonField =
        {
            WINDOW_WIDTH - squareSize - margin,
            static_cast<float>(yOffset),
            squareSize,
            rectangleHeight
        };

        // Zwiêkszenie pozycji dla kolejnego przycisku
        yOffset += rectangleHeight + 10;
    }

    ~UpgradeButton()
    {
        if (texture)
        {
            SDL_DestroyTexture(texture);
        }
    }

    void incrementScore();

    void upgradeScoreValue()
    {
        incrementValue += incrementValueChange;
    }

    void render(SDL_Renderer* renderer)
    {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &upgradeButtonField);

        if (texture)
        {
            textRect.x = upgradeButtonField.x + (upgradeButtonField.w - textRect.w) / 2;
            textRect.y = upgradeButtonField.y + (upgradeButtonField.h - textRect.h) / 2;

            SDL_RenderTexture(renderer, texture, nullptr, &textRect);
        }
    }

    float getX() const { return upgradeButtonField.x; }
    float getY() const { return upgradeButtonField.y; }
    float getW() const { return upgradeButtonField.w; }
    float getH() const { return upgradeButtonField.h; }

    void activate() { isObjectUpgrading = true; }
    void deactivate() { isObjectUpgrading = false; }
    bool isActive() const { return isObjectUpgrading; }
    float getIncrementValue() const { return incrementValue; }
    float getUpgradeCost() const { return upgradeCost; }
    float getIncrementValueChange() const { return incrementValueChange; }

private:
    static float yOffset; // Przechowuje przesuniêcie w osi Y dla kolejnych przycisków
    SDL_FRect upgradeButtonField{};
    float incrementValueChange;
};

float UpgradeButton::yOffset = 100.0f;
Uint32 UpgradeButton::lastUpdateTime = 0;

std::string getCurrentDateTime()
{
    auto now = std::chrono::system_clock::now();
    std::time_t in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%FT%T%z");

    return ss.str();
}

std::time_t parseDateTime(const std::string& dateTimeStr)
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

// zapis -------------------------------
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

void loadGameState(GameState& gameState)
{
    std::ifstream file("savegame.json");
    if (!file)
    {
        std::cout << "Brak pliku zapisu! Tworzenie nowej gry." << std::endl;
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

static GameState gameState;

void UpgradeButton::incrementScore()
{
    if (isObjectUpgrading)
    {
        gameState.score += incrementValue;
    }
}

void renderButtons(SDL_Renderer* renderer)
{
    for (auto& button : gameState.buttons)
    {
        button.render(renderer);
    }
}

void createButtons()
{
    if (gameState.buttons.empty())
    {
        gameState.buttons.emplace_back(50, 0.0f, 0.1f);
        gameState.buttons.emplace_back(100, 0.0f, 0.5f);
        gameState.buttons.emplace_back(200, 0.0f, 1.0f);
    }
}

float incrementValueCheck()
{
    float incrementValue = 0.0f;

    for (const auto& button : gameState.buttons)
    {
        if (button.isActive())
        {
            incrementValue += button.getIncrementValue();
        }
    }

    return incrementValue;
}

void createDragonButtons()
{
    if (gameState.dragonButtons.empty())
    {
        gameState.dragonButtons.emplace_back(10, 1.5f);
        gameState.dragonButtons.emplace_back(30, 2.5f);
    }
}

void renderDragonButtons(SDL_Renderer* renderer)
{
    for (auto& dragonButton : gameState.dragonButtons)
    {
        dragonButton.render(renderer);
    }
}

void createDragons()
{
    if (gameState.dragons.empty())
    {
        gameState.dragons.emplace_back(10);
    }
}

void renderDragons(SDL_Renderer* renderer)
{
    for (auto& dragon : gameState.dragons)
    {
        dragon.render(renderer);
    }
}

void updateInterval()
{
    if (SDL_GetTicks() - lastSaveTime > autoSaveInterval)
    {
        saveGameState(gameState);
        lastSaveTime = SDL_GetTicks();
    }
}

void updateScoreText()
{
    if (texture)
    {
        SDL_DestroyTexture(texture); // Usuwamy star¹ teksturê
        texture = NULL;
    }

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2) << gameState.score;
    gameState.s_score = stream.str();

    SDL_Color color = { 0, 0, 0, 255 };
    SDL_Surface* text = TTF_RenderText_Blended(font, gameState.s_score.c_str(), 0, color);

    if (text)
    {
        texture = SDL_CreateTextureFromSurface(renderer, text);
        SDL_DestroySurface(text);
    }
}

void updateIncrementValueText()
{
    float incrementValue = incrementValueCheck();

    if (incrementValueTexture)
    {
        SDL_DestroyTexture(incrementValueTexture); // Usuwamy star¹ teksturê
        incrementValueTexture = NULL;
    }

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2) << incrementValue;
    std::string incrementValueS = stream.str();

    SDL_Color color = { 0, 0, 0, 255 };
    SDL_Surface* text = TTF_RenderText_Blended(incrementValueFont, incrementValueS.c_str(), 0, color);

    if (text)
    {
        incrementValueTexture = SDL_CreateTextureFromSurface(renderer, text);
        SDL_DestroySurface(text);
    }
}

void renderScoreText()
{
    if (!texture) return;

    float w = 0, h = 0;
    SDL_GetTextureSize(texture, &w, &h);

    SDL_FRect dst;
    dst.w = w;
    dst.h = h;
    dst.x = (WINDOW_WIDTH - w) / 2; // centrowanie
    dst.y = (WINDOW_HEIGHT * 0.02f);

    SDL_RenderTexture(renderer, texture, NULL, &dst);
}

void renderIncrementValueText()
{
    if (!texture) return;

    float w = 0, h = 0;
    SDL_GetTextureSize(incrementValueTexture, &w, &h);

    SDL_FRect dst;
    dst.w = w;
    dst.h = h;
    dst.x = (WINDOW_WIDTH - w) / 2; // Centrowanie
    dst.y = (WINDOW_HEIGHT * 0.075f);  // Poni¿ej g³ównego tekstu, zmniejszaj¹c y

    SDL_RenderTexture(renderer, incrementValueTexture, NULL, &dst);
}

void updateHealthValueText()
{
    float healthValue{};

    for (auto& dragon : gameState.dragons)
    {
        healthValue = dragon.health;
    }

    if (healthValueTexture)
    {
        SDL_DestroyTexture(healthValueTexture); // Usuwamy star¹ teksturê
        healthValueTexture = NULL;
    }

    std::ostringstream stream;

    if (healthValue == 0)
    {
        stream << "Dragon has been slayed!";
    }

    else
    {
        stream << "HP: " << std::fixed << std::setprecision(2) << healthValue;
    }
    std::string incrementValueS = stream.str();

    SDL_Color color = { 0, 0, 0, 255 };
    SDL_Surface* text = TTF_RenderText_Blended(HPValueFont, incrementValueS.c_str(), 0, color);

    if (text)
    {
        healthValueTexture = SDL_CreateTextureFromSurface(renderer, text);
        SDL_DestroySurface(text);
    }
}

void renderHealthValueText()
{
    if (!healthValueTexture) return;

    float w = 0, h = 0;
    SDL_GetTextureSize(healthValueTexture, &w, &h);

    SDL_FRect dst;
    dst.w = w;
    dst.h = h;
    dst.x = (WINDOW_WIDTH - w) / 2; // centrowanie
    dst.y = (WINDOW_HEIGHT * 0.90f);

    SDL_RenderTexture(renderer, healthValueTexture, NULL, &dst);
}

void renderHealthBar(Dragon& dragon, SDL_Renderer* renderer)
{
    float barHeight = 40.0f; // Wysokoœæ paska zdrowia
    float barWidth = WINDOW_WIDTH * 0.5f; // Szerokoœæ paska zdrowia bêdzie 50% szerokoœci okna

    float healthPercentage = dragon.health / dragon.baseHealth;
    float currentBarWidth = barWidth * healthPercentage; // Szerokoœæ paska na podstawie zdrowia

    float x = (WINDOW_WIDTH - barWidth) / 2.0f; // Centrowanie w poziomie
    float y = (WINDOW_HEIGHT * 0.90f);

    SDL_FRect backgroundBar = { x, y, barWidth, barHeight };
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // Szary kolor
    SDL_RenderFillRect(renderer, &backgroundBar);

    SDL_FRect foregroundBar = { x, y, currentBarWidth, barHeight };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Czerwony kolor
    SDL_RenderFillRect(renderer, &foregroundBar);

}

void renderDragonCoinText()
{
    if (!dragonCoinTexture) return;

    float w = 0, h = 0;
    SDL_GetTextureSize(dragonCoinTexture, &w, &h);

    SDL_FRect dst;
    dst.w = w;
    dst.h = h;
    dst.x = (WINDOW_WIDTH * 0.87f); // Centrowanie
    dst.y = (WINDOW_HEIGHT * 0.02f);  // Poni¿ej g³ównego tekstu, zmniejszaj¹c y

    float coinWidth, coinHeight;
    SDL_GetTextureSize(dragonCoin, &coinWidth, &coinHeight);

    float scale = .07f;
    float newCoinWidth = static_cast<float>(coinWidth * scale);
    float newCoinHeight = static_cast<float>(coinHeight * scale);

    float posX = (WINDOW_WIDTH * 0.83f);
    float posY = (WINDOW_HEIGHT * 0.005f);

    SDL_FRect dragonField = { posX, posY, static_cast<float>(newCoinWidth), static_cast<float>(newCoinHeight) };
    SDL_RenderTexture(renderer, dragonCoin, NULL, &dragonField);

    SDL_RenderTexture(renderer, dragonCoinTexture, NULL, &dst);
}

void updateDragonCoinText(SDL_Renderer* renderer)
{
    float dragonCoinsValue = gameState.dragonCoins;

    if (dragonCoinTexture)
    {
        SDL_DestroyTexture(dragonCoinTexture); // Usuwamy star¹ teksturê
        dragonCoinTexture = NULL;
    }

    std::ostringstream stream;
    stream << "Dragon Coins: " << std::fixed << std::setprecision(0) << dragonCoinsValue;
    std::string incrementValueS = stream.str();

    SDL_Color color = { 0, 0, 0, 255 };
    SDL_Surface* text = TTF_RenderText_Blended(incrementValueFont, incrementValueS.c_str(), 0, color);

    if (text)
    {
        dragonCoinTexture = SDL_CreateTextureFromSurface(renderer, text);
        SDL_DestroySurface(text);
    }
}

void updateButtonText(SDL_Renderer* renderer)
{
    for (auto& button : gameState.buttons)
    {
        if (button.texture)
        {
            SDL_DestroyTexture(button.texture);
            button.texture = nullptr;
        }
    }

    SDL_Color color = { 0, 0, 0, 255 };

    for (auto& button : gameState.buttons)
    {
        std::ostringstream stream;
        stream << "Cost: " << std::fixed << std::setprecision(2) << button.upgradeCost;
        std::string textString = stream.str();

        SDL_Surface* textSurface = TTF_RenderText_Blended(upgradeValueFont, textString.c_str(), 0, color);
        if (textSurface)
        {
            button.texture = SDL_CreateTextureFromSurface(renderer, textSurface);

            button.textRect.w = textSurface->w;
            button.textRect.h = textSurface->h;

            SDL_DestroySurface(textSurface);
        }
    }
}

void updateDragonButtonText(SDL_Renderer* renderer)
{
    for (auto& dragonButton : gameState.dragonButtons)
    {
        if (dragonButton.texture)
        {
            SDL_DestroyTexture(dragonButton.texture);
            dragonButton.texture = nullptr;
        }
    }

    SDL_Color color = { 0, 0, 0, 255 };

    for (auto& dragonButton : gameState.dragonButtons)
    {
        std::ostringstream stream;
        stream << "Cost: " << std::fixed << std::setprecision(2) << dragonButton.dragonUpgradeCost;
        std::string textString = stream.str();

        SDL_Surface* textSurface = TTF_RenderText_Blended(upgradeValueFont, textString.c_str(), 0, color);
        if (textSurface)
        {
            dragonButton.texture = SDL_CreateTextureFromSurface(renderer, textSurface);

            dragonButton.textRect.w = textSurface->w;
            dragonButton.textRect.h = textSurface->h;

            SDL_DestroySurface(textSurface);
        }
    }
}

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    loadGameState(gameState);

    //SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    SDL_Color color = { 255, 255, 255, SDL_ALPHA_OPAQUE };
    SDL_Surface* text;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Dragon Clicker", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!TTF_Init())
    {
        SDL_Log("Couldn't initialise SDL_ttf: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    font = TTF_OpenFont("EagleLake-Regular.ttf", 32.0f);
    if (!font)
    {
        SDL_Log("Couldn't open font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    incrementValueFont = TTF_OpenFont("EagleLake-Regular.ttf", 16.0f);
    if (!incrementValueFont)
    {
        SDL_Log("Couldn't open incrementValue font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    HPValueFont = TTF_OpenFont("EagleLake-Regular.ttf", 24.0f);
    if (!HPValueFont)
    {
        SDL_Log("Couldn't open incrementValue font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    upgradeValueFont = TTF_OpenFont("EagleLake-Regular.ttf", 20.0f);
    if (!upgradeValueFont)
    {
        SDL_Log("Couldn't open incrementValue font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    text = TTF_RenderText_Blended(font, gameState.s_score.c_str(), 10, color);
    if (text)
    {
        texture = SDL_CreateTextureFromSurface(renderer, text);
        SDL_DestroySurface(text);
    }

    if (!texture)
    {
        SDL_Log("Couldn't create text: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    backgroundImage = IMG_LoadTexture(renderer, "background.jpeg");
    dragon1 = IMG_LoadTexture(renderer, "dragon1.png");
    dragonCoin = IMG_LoadTexture(renderer, "coin.png");

    if (!backgroundImage || !dragon1 || !dragonCoin)
    {
        std::cerr << "B³¹d wczytywania obrazu: " << SDL_GetError() << std::endl;
        return SDL_APP_FAILURE;
    }

    updateScoreText();
    createButtons();
    createDragons();
    createDragonButtons();

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        saveGameState(gameState);
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        for (auto& dragon : gameState.dragons)
        {
            if (mouseX >= dragon.getX() && mouseX <= dragon.getX() + dragon.getW() &&
                mouseY >= dragon.getY() && mouseY <= dragon.getY() + dragon.getH())
            {
                //std::cout << "Dragon health: " << dragon.health << std::endl;

                if (dragon.health <= 0)
                {
                    //std::cout << "You slayed a dragon!" << std::endl;
                    dragon.health = dragon.baseHealth * 1.5f;
                    dragon.baseHealth = dragon.health;
                    gameState.dragonCoins += dragon.dragonSlayedPrice;
                    dragon.dragonSlayedPrice *= 1.25f;
                }

                gameState.score += gameState.incrementScore;
                dragon.health -= gameState.incrementScore;
                updateScoreText();
            }
        }

        for (auto& button : gameState.buttons)
        {
            if (mouseX >= button.getX() && mouseX <= button.getX() + button.getW() &&
                mouseY >= button.getY() && mouseY <= button.getY() + button.getH())
            {
                if (!button.isObjectUpgrading)
                {
                    button.isObjectUpgrading = true;
                    UpgradeButton::lastUpdateTime = SDL_GetTicks();
                    // std::cout << "Ulepszanie aktywowane!" << std::endl;

                    if (gameState.score >= button.upgradeCost)
                    {
                        gameState.score -= button.upgradeCost;
                        button.upgradeScoreValue(); // Zwiêkszamy incrementValue
                        button.upgradeCost *= 1.2f;  // Zwiêkszamy koszt ulepszania
                    }
                }
                else
                {
                    if (gameState.score < button.upgradeCost)
                    {
                        return SDL_APP_CONTINUE;  /* carry on with the program! */
                    }

                    else if (gameState.score >= button.upgradeCost)
                    {
                        gameState.score -= button.upgradeCost;
                        button.upgradeScoreValue(); // Zwiêkszamy incrementValue
                        button.upgradeCost *= 1.2f;  // Zwiêkszamy koszt ulepszania
                    }
                }
            }
        }

        for (auto& dragonButton : gameState.dragonButtons)
        {
            if (mouseX >= dragonButton.getX() && mouseX <= dragonButton.getX() + dragonButton.getW() &&
                mouseY >= dragonButton.getY() && mouseY <= dragonButton.getY() + dragonButton.getH())
            {
                if (gameState.dragonCoins >= dragonButton.dragonUpgradeCost)
                {
                    gameState.incrementScore *= dragonButton.multiplier;
                    gameState.dragonCoins -= dragonButton.dragonUpgradeCost;
                    dragonButton.dragonUpgradeCost *= 2.5f;
                }
            }
        }

        updateScoreText();
        updateIncrementValueText();
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    static Uint32 lastUpdateTime = SDL_GetTicks();

    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, backgroundImage, NULL, NULL);

    renderDragons(renderer);

    for (auto& dragon : gameState.dragons)
    {
        renderHealthBar(dragon, renderer); // Renderuj pasek zdrowia
    }

    renderHealthValueText();
    updateHealthValueText();
    renderScoreText();
    renderIncrementValueText();
    renderButtons(renderer);
    renderDragonButtons(renderer);
    updateButtonText(renderer);
    renderDragonCoinText();
    updateDragonCoinText(renderer);
    updateDragonButtonText(renderer);

    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastUpdateTime >= 1000)
    {
        for (auto& button : gameState.buttons)
        {
            if (button.isActive())
            {
                // std::cout << "Before increment: score = " << score << ", incrementValue = " << button.getIncrementValue() << std::endl;
                button.incrementScore(); // Zwiêkszamy wynik
                //std::cout << "After increment: score = " << score << std::endl;
                lastUpdateTime = currentTime;
            }
        }

        updateScoreText();
    }

    if (!gameState.lastSaveTime.empty() && currentTime - lastSaveTime >= autoSaveInterval)
    {
        try
        {
            std::time_t lastSave = parseDateTime(gameState.lastSaveTime);
            std::time_t now = std::time(0);
            double timeDiffSeconds = difftime(now, lastSave);
            long double timeAFKHours = timeDiffSeconds / 3600.0f;

            long double afkBonus = std::min(1000.0L, static_cast<long double>(gameState.incrementScore * timeAFKHours * 0.25f));
            gameState.score += afkBonus;
            updateScoreText();
        }

        catch (const std::runtime_error& e)
        {
            std::cerr << "Error calulating AFK: " << e.what() << "\n";
        }

        saveGameState(gameState);
        lastSaveTime = currentTime;
        gameState.lastSaveTime = getCurrentDateTime();
    }

    else if (gameState.lastSaveTime.empty())
    {
        gameState.lastSaveTime = getCurrentDateTime();
    }

    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    if (font)
    {
        TTF_CloseFont(font);
    }

    if (texture)
    {
        SDL_DestroyTexture(texture);
    }

    if (backgroundImage)
    {
        SDL_DestroyTexture(backgroundImage);
    }

    if (dragon1)
    {
        SDL_DestroyTexture(dragon1);
    }

    TTF_Quit();
    SDL_Quit();
}