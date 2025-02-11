#include "Headers/includeFunctions.hpp"
#include "Headers/global.hpp"

void createDragonButtons()
{
    if (gameState.dragonButtons.empty())
    {
        gameState.dragonButtons.emplace_back(10, 1.5f);
        gameState.dragonButtons.emplace_back(30, 2.5f);
    }
}

void renderDragonButtons(SDL_Renderer* renderer)
{
    if (!isDragonUpgradeSidePanelVisible) return;

    for (auto& dragonButton : gameState.dragonButtons)
    {
        dragonButton.render(renderer);
    }
}