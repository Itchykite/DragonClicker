#pragma once

#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "Headers/DragonClass.hpp"
#include "Headers/UpgradeButtonClass.hpp"
#include "Headers/DragonUpgradeClass.hpp"
#include "Headers/GameStateStruct.hpp"

extern void loadGameState(GameState& gameState);
extern void saveGameState(const GameState& gameState);

extern void renderDragonCoinText();
extern void updateDragonCoinText(SDL_Renderer* renderer);

extern void createDragons();
extern void renderDragons(SDL_Renderer* renderer);

extern void createDragonButtons();
extern void renderDragonButtons(SDL_Renderer* renderer);

extern void renderHealthValueText();
extern void updateHealthValueText();
extern void renderHealthBar(Dragon& dragon, SDL_Renderer* renderer);

extern void renderIncrementValueText();
extern void updateIncrementValueText();
extern float incrementValueCheck();

extern void renderScoreText();
extern void updateScoreText();

extern void createButtons();
extern void renderButtons(SDL_Renderer* renderer);

extern void updateButtonText(SDL_Renderer* renderer);
extern void updateDragonButtonText(SDL_Renderer* renderer);
