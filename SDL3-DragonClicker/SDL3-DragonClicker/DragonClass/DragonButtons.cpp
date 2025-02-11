#include "includeFunctions.hpp"
#include "global.hpp"

void createDragons()
{
    if (gameState.dragons.empty())
    {
        gameState.dragons.emplace_back(10);
    }
}

void renderDragons(SDL_Renderer* renderer)
{
    for (auto& dragon : gameState.dragons)
    {
        dragon.render(renderer);
    }
}