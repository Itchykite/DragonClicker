#include "global.hpp"
#include "includeHeaders.hpp"
#include "ToggleButtonClass.hpp"
#include "DragonUpgradeToggleButtonClass.hpp"
#include "GameStateStruct.hpp"

int WINDOW_WIDTH = 1920;
int WINDOW_HEIGHT = 1080;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;
SDL_Texture* incrementValueTexture = NULL;
SDL_Texture* healthValueTexture = NULL;
SDL_Texture* dragonCoinTexture = NULL;
TTF_Font* font = NULL;
TTF_Font* incrementValueFont = NULL;
TTF_Font* HPValueFont = NULL;
TTF_Font* upgradeValueFont = NULL;

SDL_Texture* backgroundImage = nullptr;
SDL_Texture* dragon1 = nullptr;
SDL_Texture* dragonCoin = nullptr;
SDL_Texture* shopTexture = nullptr;
SDL_Texture* shopSwordTexture = nullptr;

Uint32 lastIncrementTime = 0;
Uint32 lastSaveTime = SDL_GetTicks();
const Uint32 autoSaveInterval = 60000;
bool isSidePanelVisible = false;
bool isDragonUpgradeSidePanelVisible = false;

ToggleButton upgradeToggleButton;
DragonUpgradeToggleButton dragonUpgradeToggleButton;
GameState gameState;