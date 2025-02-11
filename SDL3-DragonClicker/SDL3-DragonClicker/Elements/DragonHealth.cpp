#include "Headers/includeFunctions.hpp"
#include "Headers/global.hpp"

void renderHealthValueText()
{
    if (!healthValueTexture) return;

    float w = 0, h = 0;
    SDL_GetTextureSize(healthValueTexture, &w, &h);

    SDL_FRect dst;
    dst.w = w;
    dst.h = h;
    dst.x = (WINDOW_WIDTH - w) / 2; // centrowanie
    dst.y = (WINDOW_HEIGHT * 0.90f);

    SDL_RenderTexture(renderer, healthValueTexture, NULL, &dst);
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
        SDL_DestroyTexture(healthValueTexture); // Usuwamy star¹ teksturê
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

void renderHealthBar(Dragon& dragon, SDL_Renderer* renderer)
{
    float barHeight = 40.0f; // Wysokoœæ paska zdrowia
    float barWidth = WINDOW_WIDTH * 0.5f; // Szerokoœæ paska zdrowia bêdzie 50% szerokoœci okna

    float healthPercentage = dragon.health / dragon.baseHealth;
    float currentBarWidth = barWidth * healthPercentage; // Szerokoœæ paska na podstawie zdrowia

    float x = (WINDOW_WIDTH - barWidth) / 2.0f; // Centrowanie w poziomie
    float y = (WINDOW_HEIGHT * 0.90f);

    SDL_FRect backgroundBar = { x, y, barWidth, barHeight };
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // Szary kolor
    SDL_RenderFillRect(renderer, &backgroundBar);

    SDL_FRect foregroundBar = { x, y, currentBarWidth, barHeight };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Czerwony kolor
    SDL_RenderFillRect(renderer, &foregroundBar);
}