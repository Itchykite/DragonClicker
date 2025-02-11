#include "Headers/DragonUpgradeClass.hpp"
#include "Headers/global.hpp"

DragonUpgrades::DragonUpgrades(long int dragonUpgradeCost, long double multiplier) : dragonUpgradeCost(dragonUpgradeCost), multiplier(multiplier)
{
    float clickFieldSizePercentage = 0.25f;
    float squareSize = static_cast<float>(std::min(WINDOW_WIDTH, WINDOW_HEIGHT) * clickFieldSizePercentage);
    int margin = 20;
    float rectangleHeight = squareSize / 4;

    // Automatyczne pozycjonowanie przycisków jeden pod drugim po prawej stronie
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

DragonUpgrades::~DragonUpgrades()
{
    if (texture)
    {
        SDL_DestroyTexture(texture);
    }
}

void DragonUpgrades::render(SDL_Renderer* renderer)
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