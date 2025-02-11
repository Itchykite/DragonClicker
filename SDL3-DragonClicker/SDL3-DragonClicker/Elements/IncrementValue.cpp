#include "includeFunctions.hpp"
#include "global.hpp"

void renderIncrementValueText()
{
    if (!texture) return;

    float w = 0, h = 0;
    SDL_GetTextureSize(incrementValueTexture, &w, &h);

    SDL_FRect dst;
    dst.w = w;
    dst.h = h;
    dst.x = (WINDOW_WIDTH - w) / 2; // Centrowanie
    dst.y = (WINDOW_HEIGHT * 0.075f);  // Poni¿ej g³ównego tekstu, zmniejszaj¹c y

    SDL_RenderTexture(renderer, incrementValueTexture, NULL, &dst);
}

void updateIncrementValueText()
{
    float incrementValue = incrementValueCheck();

    if (incrementValueTexture)
    {
        SDL_DestroyTexture(incrementValueTexture); // Usuwamy star¹ teksturê
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