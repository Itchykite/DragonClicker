#include "includeHeaders.hpp"
#include "ToggleButtonClass.hpp"
#include "DragonUpgradeToggleButtonClass.hpp"
#include "GameStateStruct.hpp"

static int WINDOW_WIDTH = 1920;
static int WINDOW_HEIGHT = 1080;

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* texture = NULL;
static SDL_Texture* incrementValueTexture = NULL;
static SDL_Texture* healthValueTexture = NULL;
static SDL_Texture* dragonCoinTexture = NULL;
static TTF_Font* font = NULL;
static TTF_Font* incrementValueFont = NULL;
static TTF_Font* HPValueFont = NULL;
static TTF_Font* upgradeValueFont = NULL;

static SDL_Texture* backgroundImage = nullptr;
static SDL_Texture* dragon1 = nullptr;
static SDL_Texture* dragonCoin = nullptr;
static SDL_Texture* shopTexture = nullptr;
static SDL_Texture* shopSwordTexture = nullptr;

Uint32 lastIncrementTime = 0;
Uint32 lastSaveTime = SDL_GetTicks();
const Uint32 autoSaveInterval = 60000;
bool isSidePanelVisible = false;
bool isDragonUpgradeSidePanelVisible = false;

static ToggleButton upgradeToggleButton;
static DragonUpgradeToggleButton dragonUpgradeToggleButton;
static GameState gameState;