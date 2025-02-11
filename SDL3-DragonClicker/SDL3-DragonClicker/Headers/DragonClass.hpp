#pragma once
#include "headers.hpp"
#include "global.hpp"

class Dragon
{
public:
    long double health;
    long double baseHealth = 100;
    double dragonSlayedPrice = 0;

    Dragon(double dragonSlayedPrice) : dragonSlayedPrice(dragonSlayedPrice) {}

    Dragon(double dragonSlayedPrice, long double health, long double baseHealth)
        : dragonSlayedPrice(dragonSlayedPrice), health(health), baseHealth(baseHealth) {}

    void render(SDL_Renderer* renderer);

    float getX() const { return clickField.x; }
    float getY() const { return clickField.y; }
    float getW() const { return clickField.w; }
    float getH() const { return clickField.h; }

private:
    SDL_FRect clickField{};
};