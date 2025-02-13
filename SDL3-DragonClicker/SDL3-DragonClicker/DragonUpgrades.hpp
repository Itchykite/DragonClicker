#pragma once

#include <SDL3/SDL.h>

class DragonUpgrades
{
public:
    SDL_Texture* texture;
    SDL_FRect textRect;

    long double multiplier;
    long int dragonUpgradeCost;

    DragonUpgrades(long int dragonUpgradeCost, long double multiplier);
    ~DragonUpgrades();

    void render(SDL_Renderer* renderer) const;

    float getX() const;
    float getY() const;
    float getW() const;
    float getH() const;
    long double getMultiplier() const;
    long int getDragonUpgradeCost() const;

    void setVisible(bool visible);
    bool isVisible() const;

private:
    static float yOffset;
    SDL_FRect upgradeButtonField;
};