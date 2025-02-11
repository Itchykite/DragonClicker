#pragma once
#include "includeHeaders.hpp"
#include "ToggleButtonClass.hpp"
#include "DragonUpgradeToggleButtonClass.hpp"
#include "GameStateStruct.hpp"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* texture;
extern SDL_Texture* incrementValueTexture;
extern SDL_Texture* healthValueTexture;
extern SDL_Texture* dragonCoinTexture;
extern TTF_Font* font;
extern TTF_Font* incrementValueFont;
extern TTF_Font* HPValueFont;
extern TTF_Font* upgradeValueFont;

extern SDL_Texture* backgroundImage;
extern SDL_Texture* dragon1;
extern SDL_Texture* dragonCoin;
extern SDL_Texture* shopTexture;
extern SDL_Texture* shopSwordTexture;

extern Uint32 lastIncrementTime;
extern Uint32 lastSaveTime;
extern const Uint32 autoSaveInterval;
extern bool isSidePanelVisible;
extern bool isDragonUpgradeSidePanelVisible;

extern ToggleButton upgradeToggleButton;
extern DragonUpgradeToggleButton dragonUpgradeToggleButton;
extern GameState gameState;