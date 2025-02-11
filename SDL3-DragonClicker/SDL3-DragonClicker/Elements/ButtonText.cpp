#include "Headers/includeHeaders.hpp"
#include "Headers/global.hpp"

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