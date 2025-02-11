#include "Headers/includeHeaders.hpp"
#include "Headers/global.hpp"

UpgradeButton::UpgradeButton(float upgradeCost, float incrementValue, float incrementValueChange)
    : upgradeCost(upgradeCost), incrementValue(incrementValue), incrementValueChange(incrementValueChange), isObjectUpgrading(false)
{
    float clickFieldSizePercentage = 0.25f;
    float squareSize = static_cast<float>(std::min(WINDOW_WIDTH, WINDOW_HEIGHT) * clickFieldSizePercentage);
    int margin = 20;
    float rectangleHeight = squareSize / 4;

    // Automatyczne pozycjonowanie przycisków jeden pod drugim
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

UpgradeButton::~UpgradeButton()
{
    if (texture)
    {
        SDL_DestroyTexture(texture);
    }
}

void UpgradeButton::upgradeScoreValue()
{
    incrementValue += incrementValueChange;
}

void UpgradeButton::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &upgradeButtonField);

    if (texture)
    {
        textRect.x = upgradeButtonField.x + (upgradeButtonField.w - textRect.w) / 2;
        textRect.y = upgradeButtonField.y + (upgradeButtonField.h - textRect.h) / 2;

        SDL_RenderTexture(renderer, texture, nullptr, &textRect);
    }
}

void UpgradeButton::incrementScore()
{
    if (isObjectUpgrading)
    {
        gameState.score += incrementValue;
    }
}

float UpgradeButton::yOffset = 100.0f;
Uint32 UpgradeButton::lastUpdateTime = 0;