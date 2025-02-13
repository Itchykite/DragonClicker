#include "UpgradeButton.hpp"
#include "GameState.hpp"
#include <algorithm>

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern GameState gameState;

float UpgradeButton::yOffset = 20.0f;
Uint32 UpgradeButton::lastUpdateTime = 0;

UpgradeButton::UpgradeButton(float upgradeCost, float incrementValue, float incrementValueChange)
    : upgradeCost(upgradeCost), incrementValue(incrementValue), incrementValueChange(incrementValueChange), texture(nullptr)
{
    float clickFieldSizePercentage = 0.25f;
    float squareSize = static_cast<float>(std::min(WINDOW_WIDTH, WINDOW_HEIGHT) * clickFieldSizePercentage);
    int margin = 20;
    float rectangleHeight = squareSize / 4;

    upgradeButtonField = { WINDOW_WIDTH - squareSize - margin, yOffset, squareSize, rectangleHeight };
    yOffset += rectangleHeight + 10;
}

UpgradeButton::~UpgradeButton()
{
    if (texture)
    {
        SDL_DestroyTexture(texture);
    }
}

void UpgradeButton::incrementScore()
{
    if (isObjectUpgrading)
    {
		gameState.score += incrementValue;
    }
}

void UpgradeButton::upgradeScoreValue()
{
    incrementValue += incrementValueChange;
}

void UpgradeButton::render(SDL_Renderer* renderer) const
{
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &upgradeButtonField);

    if (texture)
    {
        SDL_FRect textRectCopy = textRect;
        textRectCopy.x = upgradeButtonField.x + (upgradeButtonField.w - textRectCopy.w) / 2;
        textRectCopy.y = upgradeButtonField.y + (upgradeButtonField.h - textRectCopy.h) / 2;
        SDL_RenderTexture(renderer, texture, nullptr, &textRectCopy);
    }
}

float UpgradeButton::getX() const { return upgradeButtonField.x; }
float UpgradeButton::getY() const { return upgradeButtonField.y; }
float UpgradeButton::getW() const { return upgradeButtonField.w; }
float UpgradeButton::getH() const { return upgradeButtonField.h; }

void UpgradeButton::activate() { isObjectUpgrading = true; }
void UpgradeButton::deactivate() { isObjectUpgrading = false; }
bool UpgradeButton::isActive() const { return isObjectUpgrading; }
float UpgradeButton::getIncrementValue() const { return incrementValue; }
float UpgradeButton::getUpgradeCost() const { return upgradeCost; }
float UpgradeButton::getIncrementValueChange() const { return incrementValueChange; }