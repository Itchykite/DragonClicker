#include "DragonUpgrades.hpp"
#include "GameState.hpp"
#include <algorithm>

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern GameState gameState;

float DragonUpgrades::yOffset = 20.0f;

DragonUpgrades::DragonUpgrades(long int dragonUpgradeCost, long double multiplier)
    : dragonUpgradeCost(dragonUpgradeCost), multiplier(multiplier), texture(nullptr)
{
    float clickFieldSizePercentage = 0.25f;
    float squareSize = static_cast<float>(std::min(WINDOW_WIDTH, WINDOW_HEIGHT) * clickFieldSizePercentage);
    int margin = 20;
    float rectangleHeight = squareSize / 4;

    int totalButtons = gameState.dragonButtons.size();
    float totalHeight = totalButtons * (rectangleHeight + 10) - 10;

    if (yOffset == 0.0f)
    {
        yOffset = (WINDOW_HEIGHT - totalHeight) / 2;
    }

    upgradeButtonField = { WINDOW_WIDTH - squareSize - margin, static_cast<float>(yOffset), squareSize, rectangleHeight };
    yOffset += rectangleHeight + 10;
}

DragonUpgrades::~DragonUpgrades()
{
    if (texture)
    {
        SDL_DestroyTexture(texture);
    }
}

void DragonUpgrades::render(SDL_Renderer* renderer) const
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &upgradeButtonField);

    if (texture)
    {
        SDL_FRect textRectCopy = textRect;
        textRectCopy.x = upgradeButtonField.x + (upgradeButtonField.w - textRectCopy.w) / 2;
        textRectCopy.y = upgradeButtonField.y + (upgradeButtonField.h - textRectCopy.h) / 2;
        SDL_RenderTexture(renderer, texture, nullptr, &textRectCopy);
    }
}

float DragonUpgrades::getX() const { return upgradeButtonField.x; }
float DragonUpgrades::getY() const { return upgradeButtonField.y; }
float DragonUpgrades::getW() const { return upgradeButtonField.w; }
float DragonUpgrades::getH() const { return upgradeButtonField.h; }
long double DragonUpgrades::getMultiplier() const { return multiplier; }
long int DragonUpgrades::getDragonUpgradeCost() const { return dragonUpgradeCost; }