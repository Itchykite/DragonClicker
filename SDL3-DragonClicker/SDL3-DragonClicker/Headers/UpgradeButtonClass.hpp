#pragma once
#include "headers.hpp"
#include "global.hpp"

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
        : upgradeCost(upgradeCost), incrementValue(incrementValue), incrementValueChange(incrementValueChange), isObjectUpgrading(false) {}

    ~UpgradeButton() {}

    void incrementScore();

    void upgradeScoreValue();

    void render(SDL_Renderer* renderer);

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