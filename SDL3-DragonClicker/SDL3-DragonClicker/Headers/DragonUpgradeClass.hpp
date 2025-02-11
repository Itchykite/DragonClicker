#pragma once
#include "headers.hpp"
#include "global.hpp"

class DragonUpgrades
{
public:
    SDL_Texture* texture = nullptr;
    SDL_FRect textRect{};

    long double multiplier{};
    long int dragonUpgradeCost{};

    DragonUpgrades(long int dragonUpgradeCost, long double multiplier) : dragonUpgradeCost(dragonUpgradeCost), multiplier(multiplier) {}

    ~DragonUpgrades() {}

    void render(SDL_Renderer* renderer);

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