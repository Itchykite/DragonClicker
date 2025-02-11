#include "Headers/includeFunctions.hpp"
#include "Headers/global.hpp"

void renderDragonCoinText()
{
    if (!dragonCoinTexture) return;

    float w = 0, h = 0;
    SDL_GetTextureSize(dragonCoinTexture, &w, &h);

    SDL_FRect dst;
    dst.w = w;
    dst.h = h;
    dst.x = (WINDOW_WIDTH * 0.05f); // Centrowanie
    dst.y = (WINDOW_HEIGHT * 0.02f);  // Poni¿ej g³ównego tekstu, zmniejszaj¹c y

    float coinWidth, coinHeight;
    SDL_GetTextureSize(dragonCoin, &coinWidth, &coinHeight);

    float scale = .07f;
    float newCoinWidth = static_cast<float>(coinWidth * scale);
    float newCoinHeight = static_cast<float>(coinHeight * scale);

    float posX = (WINDOW_WIDTH * 0.01f);
    float posY = (WINDOW_HEIGHT * 0.01f);

    SDL_FRect dragonField = { posX, posY, static_cast<float>(newCoinWidth), static_cast<float>(newCoinHeight) };
    SDL_RenderTexture(renderer, dragonCoin, NULL, &dragonField);

    SDL_RenderTexture(renderer, dragonCoinTexture, NULL, &dst);
}

void updateDragonCoinText(SDL_Renderer* renderer)
{
    float dragonCoinsValue = gameState.dragonCoins;

    if (dragonCoinTexture)
    {
        SDL_DestroyTexture(dragonCoinTexture); // Usuwamy star¹ teksturê
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