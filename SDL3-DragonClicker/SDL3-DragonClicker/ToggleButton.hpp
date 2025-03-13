#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class ToggleButton
{
public:
    SDL_Texture* texture;
    SDL_FRect textRect;
    SDL_FRect buttonRect;

    ToggleButton();
    ~ToggleButton();

    void render(SDL_Renderer* renderer);
    bool isClicked(float mouseX, float mouseY);
};

class DragonUpgradeToggleButton
{
public:
    SDL_Texture* texture;
    SDL_FRect textRect;
    SDL_FRect buttonRect;

    DragonUpgradeToggleButton();
    ~DragonUpgradeToggleButton();

    void render(SDL_Renderer* renderer);
    bool isClicked(float mouseX, float mouseY);
};

