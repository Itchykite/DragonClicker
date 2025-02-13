#pragma once

#include <SDL3/SDL.h>

class UpgradeButton
{
public:
    SDL_Texture* texture;
    SDL_FRect textRect;

    bool isObjectUpgrading;
    float incrementValue;
    float upgradeCost;
    static Uint32 lastUpdateTime;

    UpgradeButton(float upgradeCost, float incrementValue, float incrementValueChange);
    ~UpgradeButton();

    void incrementScore();
    void upgradeScoreValue();
    void render(SDL_Renderer* renderer) const;

    float getX() const;
    float getY() const;
    float getW() const;
    float getH() const;

    void activate();
    void deactivate();
    bool isActive() const;
    float getIncrementValue() const;
    float getUpgradeCost() const;
    float getIncrementValueChange() const;

    void setVisible(bool visible);
    bool isVisible() const;

private:
    static float yOffset;
    SDL_FRect upgradeButtonField;
    float incrementValueChange;
};