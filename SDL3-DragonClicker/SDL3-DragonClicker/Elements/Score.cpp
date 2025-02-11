#include "Headers/includeFunctions.hpp"
#include "Headers/global.hpp"

void renderScoreText()
{
    if (!texture) return;

    float w = 0, h = 0;
    SDL_GetTextureSize(texture, &w, &h);

    SDL_FRect dst;
    dst.w = w;
    dst.h = h;
    dst.x = (WINDOW_WIDTH - w) / 2; // centrowanie
    dst.y = (WINDOW_HEIGHT * 0.02f);

    SDL_RenderTexture(renderer, texture, NULL, &dst);
}

void updateScoreText()
{
    if (texture)
    {
        SDL_DestroyTexture(texture); // Usuwamy star¹ teksturê
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