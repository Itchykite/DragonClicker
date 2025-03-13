#pragma once

#include <SDL3/SDL.h>
#include "GameState.hpp"
#include "Renderer.hpp"

void updateScoreText();
void updateIncrementValueText();
void updateHealthValueText();
void updateDragonCoinText(SDL_Renderer* renderer);
void updateButtonText(SDL_Renderer* renderer);
void updateDragonButtonText(SDL_Renderer* renderer);
void updateDragonUpgradeToggleButtonText(SDL_Renderer* renderer);
void updateToggleButtonText(SDL_Renderer* renderer);
float incrementValueCheck();