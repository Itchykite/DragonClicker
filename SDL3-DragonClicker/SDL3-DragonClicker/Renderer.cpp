#include "Renderer.hpp"
#include <sstream>
#include <iomanip>

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern SDL_Texture* texture;
extern SDL_Texture* incrementValueTexture;
extern SDL_Texture* healthValueTexture;
extern SDL_Texture* dragonCoinTexture;
extern SDL_Texture* dragonCoin;
extern TTF_Font* font;
extern TTF_Font* incrementValueFont;
extern TTF_Font* HPValueFont;
extern TTF_Font* upgradeValueFont;

void renderButtons(SDL_Renderer* renderer, const GameState& gameState)
{
    for (const auto& button : gameState.buttons)
    {
        button.render(renderer);
    }
}

void renderDragonButtons(SDL_Renderer* renderer, const GameState& gameState)
{
    for (const auto& dragonButton : gameState.dragonButtons)
    {
        dragonButton.render(renderer);
    }
}

void renderDragons(SDL_Renderer* renderer, const GameState& gameState)
{
    for (const auto& dragon : gameState.dragons)
    {
        dragon.render(renderer);
    }
}

void renderScoreText(SDL_Renderer* renderer, const GameState& gameState)
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

void renderIncrementValueText(SDL_Renderer* renderer, const GameState& gameState)
{
    if (!incrementValueTexture) return;

    float w = 0, h = 0;
    SDL_GetTextureSize(incrementValueTexture, &w, &h);

    SDL_FRect dst;
    dst.w = w;
    dst.h = h;
    dst.x = (WINDOW_WIDTH - w) / 2; // Centrowanie
    dst.y = (WINDOW_HEIGHT * 0.075f);  // Poni¿ej g³ównego tekstu, zmniejszaj¹c y

    SDL_RenderTexture(renderer, incrementValueTexture, NULL, &dst);
}

void renderHealthValueText(SDL_Renderer* renderer, const GameState& gameState)
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

void renderDragonCoinText(SDL_Renderer* renderer, const GameState& gameState)
{
    if (!dragonCoinTexture) return;

    float w = 0, h = 0;
    SDL_GetTextureSize(dragonCoinTexture, &w, &h);

    SDL_FRect dst;
    dst.w = w;
    dst.h = h;
    dst.x = (WINDOW_WIDTH * 0.87f); // Centrowanie
    dst.y = (WINDOW_HEIGHT * 0.02f);  // Poni¿ej g³ównego tekstu, zmniejszaj¹c y

    float coinWidth, coinHeight;
    SDL_GetTextureSize(dragonCoin, &coinWidth, &coinHeight);

    float scale = .07f;
    float newCoinWidth = static_cast<float>(coinWidth * scale);
    float newCoinHeight = static_cast<float>(coinHeight * scale);

    float posX = (WINDOW_WIDTH * 0.83f);
    float posY = (WINDOW_HEIGHT * 0.005f);

    SDL_FRect dragonField = { posX, posY, static_cast<float>(newCoinWidth), static_cast<float>(newCoinHeight) };
    SDL_RenderTexture(renderer, dragonCoin, NULL, &dragonField);

    SDL_RenderTexture(renderer, dragonCoinTexture, NULL, &dst);
}

void renderSidePanel(SDL_Renderer* renderer, bool isSidePanelVisible, bool isDragonUpgradeSidePanelVisible)
{
    if (isSidePanelVisible)
    {
        SDL_FRect sidePanel = { WINDOW_WIDTH - 300, 0, 300, WINDOW_HEIGHT };
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 128); // Szary kolor
        SDL_RenderFillRect(renderer, &sidePanel);
    }

    if (isDragonUpgradeSidePanelVisible)
    {
        SDL_FRect sidePanel = { WINDOW_WIDTH - 300, 0, 300, WINDOW_HEIGHT };
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 128); // Szary kolor
        SDL_RenderFillRect(renderer, &sidePanel);
    }
}

void renderHealthBar(Dragon& dragon, SDL_Renderer* renderer)
{
    float barHeight = 40.0f;
    float barWidth = WINDOW_WIDTH * 0.5f;

	float healthPercentage = dragon.health / dragon.baseHealth;
    float currentBarWidth = barWidth * healthPercentage;

    float x = (WINDOW_WIDTH - barWidth) / 2;
    float y = (WINDOW_HEIGHT * 0.90f);

	SDL_FRect backgroundBar = { x, y, barWidth, barHeight };
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
	SDL_RenderFillRect(renderer, &backgroundBar);

	SDL_FRect foregroundBar = { x, y, currentBarWidth, barHeight };
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &foregroundBar);
}