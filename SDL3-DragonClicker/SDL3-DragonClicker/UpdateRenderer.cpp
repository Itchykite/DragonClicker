#include "UpdateRenderer.hpp"
#include "GameState.hpp"
#include "ToggleButton.hpp"
#include <sstream>
#include <iomanip>

extern SDL_Texture* texture;
extern SDL_Texture* incrementValueTexture;
extern SDL_Texture* healthValueTexture;
extern SDL_Texture* dragonCoinTexture;
extern TTF_Font* font;
extern TTF_Font* incrementValueFont;
extern TTF_Font* HPValueFont;
extern TTF_Font* upgradeValueFont;
extern SDL_Renderer* renderer;
extern GameState gameState;
extern ToggleButton upgradeToggleButton;
extern DragonUpgradeToggleButton dragonUpgradeToggleButton;

void updateScoreText()
{
    if (texture)
    {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2) << gameState.score;
    gameState.s_score = stream.str();

    SDL_Color color = { 0, 0, 0, 255 };
    SDL_Surface* text = TTF_RenderText_Blended(font, gameState.s_score.c_str(), 0, color);

    if (text)
    {
        texture = SDL_CreateTextureFromSurface(renderer, text);
        SDL_DestroySurface(text);
    }
}

void updateIncrementValueText()
{
    float incrementValue = incrementValueCheck();

    if (incrementValueTexture)
    {
        SDL_DestroyTexture(incrementValueTexture);
        incrementValueTexture = NULL;
    }

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2) << incrementValue;
    std::string incrementValueS = stream.str();

    SDL_Color color = { 0, 0, 0, 255 };
    SDL_Surface* text = TTF_RenderText_Blended(incrementValueFont, incrementValueS.c_str(), 0, color);

    if (text)
    {
        incrementValueTexture = SDL_CreateTextureFromSurface(renderer, text);
        SDL_DestroySurface(text);
    }
}

void updateHealthValueText()
{
    float healthValue{};

    for (auto& dragon : gameState.dragons)
    {
        healthValue = dragon.health;
    }

    if (healthValueTexture)
    {
        SDL_DestroyTexture(healthValueTexture);
        healthValueTexture = NULL;
    }

    std::ostringstream stream;

    if (healthValue == 0)
    {
        stream << "Dragon has been slayed!";
    }
    else
    {
        stream << "HP: " << std::fixed << std::setprecision(2) << healthValue;
    }
    std::string incrementValueS = stream.str();

    SDL_Color color = { 0, 0, 0, 255 };
    SDL_Surface* text = TTF_RenderText_Blended(HPValueFont, incrementValueS.c_str(), 0, color);

    if (text)
    {
        healthValueTexture = SDL_CreateTextureFromSurface(renderer, text);
        SDL_DestroySurface(text);
    }
}

void updateDragonCoinText(SDL_Renderer* renderer)
{
    float dragonCoinsValue = gameState.dragonCoins;

    if (dragonCoinTexture)
    {
        SDL_DestroyTexture(dragonCoinTexture);
        dragonCoinTexture = NULL;
    }

    std::ostringstream stream;
    stream << "Dragon Coins: " << std::fixed << std::setprecision(0) << dragonCoinsValue;
    std::string incrementValueS = stream.str();

    SDL_Color color = { 0, 0, 0, 255 };
    SDL_Surface* text = TTF_RenderText_Blended(incrementValueFont, incrementValueS.c_str(), 0, color);

    if (text)
    {
        dragonCoinTexture = SDL_CreateTextureFromSurface(renderer, text);
        SDL_DestroySurface(text);
    }
}

void updateButtonText(SDL_Renderer* renderer)
{
    for (auto& button : gameState.buttons)
    {
        if (button.texture)
        {
            SDL_DestroyTexture(button.texture);
            button.texture = nullptr;
        }
    }

    SDL_Color color = { 0, 0, 0, 255 };

    for (auto& button : gameState.buttons)
    {
        std::ostringstream stream;
        stream << "Cost: " << std::fixed << std::setprecision(2) << button.upgradeCost;
        std::string textString = stream.str();

        SDL_Surface* textSurface = TTF_RenderText_Blended(upgradeValueFont, textString.c_str(), 0, color);
        if (textSurface)
        {
            button.texture = SDL_CreateTextureFromSurface(renderer, textSurface);

            button.textRect.w = textSurface->w;
            button.textRect.h = textSurface->h;

            SDL_DestroySurface(textSurface);
        }
    }
}

void updateDragonButtonText(SDL_Renderer* renderer)
{
    for (auto& dragonButton : gameState.dragonButtons)
    {
        if (dragonButton.texture)
        {
            SDL_DestroyTexture(dragonButton.texture);
            dragonButton.texture = nullptr;
        }
    }

    SDL_Color color = { 0, 0, 0, 255 };

    for (auto& dragonButton : gameState.dragonButtons)
    {
        std::ostringstream stream;
        stream << "Cost: " << std::fixed << std::setprecision(2) << dragonButton.dragonUpgradeCost;
        std::string textString = stream.str();

        SDL_Surface* textSurface = TTF_RenderText_Blended(upgradeValueFont, textString.c_str(), 0, color);
        if (textSurface)
        {
            dragonButton.texture = SDL_CreateTextureFromSurface(renderer, textSurface);

            dragonButton.textRect.w = textSurface->w;
            dragonButton.textRect.h = textSurface->h;

            SDL_DestroySurface(textSurface);
        }
    }
}

void updateDragonUpgradeToggleButtonText(SDL_Renderer* renderer)
{
    if (dragonUpgradeToggleButton.texture)
    {
        SDL_DestroyTexture(dragonUpgradeToggleButton.texture);
        dragonUpgradeToggleButton.texture = nullptr;
    }

    SDL_Color color = { 10, 10, 10, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, "Toggle", 0, color);
    if (textSurface)
    {
        dragonUpgradeToggleButton.texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        dragonUpgradeToggleButton.textRect.w = textSurface->w;
        dragonUpgradeToggleButton.textRect.h = textSurface->h;
        SDL_DestroySurface(textSurface);
    }
}

void updateToggleButtonText(SDL_Renderer* renderer)
{
    if (upgradeToggleButton.texture)
    {
        SDL_DestroyTexture(upgradeToggleButton.texture);
        upgradeToggleButton.texture = nullptr;
    }
    SDL_Color color = { 255, 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, "Toggle", 0, color);
    if (textSurface)
    {
        upgradeToggleButton.texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        upgradeToggleButton.textRect.w = textSurface->w;
        upgradeToggleButton.textRect.h = textSurface->h;
        SDL_DestroySurface(textSurface);
    }
}

float incrementValueCheck()
{
    float incrementValue = 0.0f;

    for (const auto& button : gameState.buttons)
    {
        if (button.isActive())
        {
            incrementValue += button.getIncrementValue();
        }
    }

    return incrementValue;
}