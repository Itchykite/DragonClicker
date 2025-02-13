#pragma once

#include <SDL3/SDL.h>

class Dragon
{
public:
    long double health;
    long double baseHealth;
    double dragonSlayedPrice;

    Dragon(double dragonSlayedPrice);
    Dragon(double dragonSlayedPrice, long double health, long double baseHealth);
    void render(SDL_Renderer* renderer) const;

    float getX() const;
    float getY() const;
    float getW() const;
    float getH() const;

private:
    SDL_FRect clickField;
};