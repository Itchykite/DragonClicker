#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "GameState.hpp"

void renderButtons(SDL_Renderer* renderer, const GameState& gameState);
void renderDragonButtons(SDL_Renderer* renderer, const GameState& gameState);
void renderDragons(SDL_Renderer* renderer, const GameState& gameState);
void renderScoreText(SDL_Renderer* renderer, const GameState& gameState);
void renderIncrementValueText(SDL_Renderer* renderer, const GameState& gameState);
void renderHealthValueText(SDL_Renderer* renderer, const GameState& gameState);
void renderDragonCoinText(SDL_Renderer* renderer, const GameState& gameState);
void renderSidePanel(SDL_Renderer* renderer, bool isSidePanelVisible, bool isDragonUpgradeSidePanelVisible);
void renderHealthBar(Dragon& dragon, SDL_Renderer* renderer);
