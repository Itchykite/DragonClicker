#include "Headers/includeFunctions.hpp"
#include "Headers/global.hpp"

void createButtons()
{
    if (gameState.buttons.empty())
    {
        gameState.buttons.emplace_back(50, 0.0f, 0.1f);
        gameState.buttons.emplace_back(100, 0.0f, 0.5f);
        gameState.buttons.emplace_back(200, 0.0f, 1.0f);
        gameState.buttons.emplace_back(500, 0.0f, 2.0f);
    }
}

void renderButtons(SDL_Renderer* renderer)
{
    if (!isSidePanelVisible) return;

    for (auto& button : gameState.buttons)
    {
        button.render(renderer);
    }
}