#include "Dragon.hpp"
#include <algorithm>

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern SDL_Texture* dragon1;

Dragon::Dragon(double dragonSlayedPrice) : dragonSlayedPrice(dragonSlayedPrice), baseHealth(100), health(baseHealth)
{
    float clickFieldSizePercentage = 0.75f;
    float squareSize = std::min(WINDOW_WIDTH, WINDOW_HEIGHT) * clickFieldSizePercentage;
    clickField = { (WINDOW_WIDTH - squareSize) / 2.0f, (WINDOW_HEIGHT - squareSize) / 2.0f, squareSize, squareSize };
}

Dragon::Dragon(double dragonSlayedPrice, long double health, long double baseHealth)
    : dragonSlayedPrice(dragonSlayedPrice), health(health), baseHealth(baseHealth)
{
    if (health == 0.0) health = baseHealth;
}

void Dragon::render(SDL_Renderer* renderer) const
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

float Dragon::getX() const { return clickField.x; }
float Dragon::getY() const { return clickField.y; }
float Dragon::getW() const { return clickField.w; }
float Dragon::getH() const { return clickField.h; }