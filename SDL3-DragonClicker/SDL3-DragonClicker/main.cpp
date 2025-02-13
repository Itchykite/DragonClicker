#define SDL_MAIN_USE_CALLBACKS 1 
#define _CRT_SECURE_NO_WARNINGS

#include <SDL3/SDL_main.h>
#include "Renderer.hpp"
#include "ToggleButton.hpp"
#include "GameState.hpp"
#include "UpdateRenderer.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <nlohmann/json.hpp>
#include <SDL3_image/SDL_image.h>

int WINDOW_WIDTH = 1920;
int WINDOW_HEIGHT = 1080;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;
SDL_Texture* incrementValueTexture = NULL;
SDL_Texture* healthValueTexture = NULL;
SDL_Texture* dragonCoinTexture = NULL;
SDL_Texture* backgroundImage = nullptr;
SDL_Texture* dragon1 = nullptr;
SDL_Texture* dragonCoin = nullptr;
TTF_Font* font = NULL;
TTF_Font* incrementValueFont = NULL;
TTF_Font* HPValueFont = NULL;
TTF_Font* upgradeValueFont = NULL;

Uint32 lastIncrementTime = 0;
Uint32 lastSaveTime = SDL_GetTicks();
const Uint32 autoSaveInterval = 60000;
bool isSidePanelVisible = false;
bool isDragonUpgradeSidePanelVisible = false;

GameState gameState;
ToggleButton upgradeToggleButton;
DragonUpgradeToggleButton dragonUpgradeToggleButton;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    loadGameState(gameState);

    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    SDL_Color color = { 255, 255, 255, SDL_ALPHA_OPAQUE };
    SDL_Surface* text;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Dragon Clicker", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    updateDragonUpgradeToggleButtonText(renderer);
    updateToggleButtonText(renderer);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    if (!TTF_Init())
    {
        SDL_Log("Couldn't initialise SDL_ttf: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    font = TTF_OpenFont("EagleLake-Regular.ttf", 32.0f);
    if (!font)
    {
        SDL_Log("Couldn't open font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    incrementValueFont = TTF_OpenFont("EagleLake-Regular.ttf", 16.0f);
    if (!incrementValueFont)
    {
        SDL_Log("Couldn't open incrementValue font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    HPValueFont = TTF_OpenFont("EagleLake-Regular.ttf", 24.0f);
    if (!HPValueFont)
    {
        SDL_Log("Couldn't open incrementValue font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    upgradeValueFont = TTF_OpenFont("EagleLake-Regular.ttf", 20.0f);
    if (!upgradeValueFont)
    {
        SDL_Log("Couldn't open incrementValue font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    text = TTF_RenderText_Blended(font, gameState.s_score.c_str(), 10, color);
    if (text)
    {
        texture = SDL_CreateTextureFromSurface(renderer, text);
        SDL_DestroySurface(text);
    }

    if (!texture)
    {
        SDL_Log("Couldn't create text: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    backgroundImage = IMG_LoadTexture(renderer, "background.jpeg");
    dragon1 = IMG_LoadTexture(renderer, "dragon1.png");
    dragonCoin = IMG_LoadTexture(renderer, "coin.png");

    if (!backgroundImage || !dragon1 || !dragonCoin)
    {
        std::cerr << "B³¹d wczytywania obrazu: " << SDL_GetError() << std::endl;
        return SDL_APP_FAILURE;
    }

    updateScoreText();
    updateIncrementValueText();
    updateButtonText(renderer);
    updateDragonButtonText(renderer);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        saveGameState(gameState);
        return SDL_APP_SUCCESS;
    }

    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        if (event->key.key == SDLK_ESCAPE)
        {
            saveGameState(gameState);
            return SDL_APP_SUCCESS;
        }
    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        if (upgradeToggleButton.isClicked(mouseX, mouseY) && isDragonUpgradeSidePanelVisible == false)
        {
            isSidePanelVisible = !isSidePanelVisible;
        }

        if (dragonUpgradeToggleButton.isClicked(mouseX, mouseY) && isSidePanelVisible == false)
        {
            isDragonUpgradeSidePanelVisible = !isDragonUpgradeSidePanelVisible;
        }

        if (isSidePanelVisible)
        {
            for (auto& button : gameState.buttons)
            {
                if (mouseX >= button.getX() && mouseX <= button.getX() + button.getW() &&
                    mouseY >= button.getY() && mouseY <= button.getY() + button.getH())
                {
                    if (!button.isObjectUpgrading)
                    {
                        button.isObjectUpgrading = true;
                        UpgradeButton::lastUpdateTime = SDL_GetTicks();

                        if (gameState.score >= button.upgradeCost)
                        {
                            gameState.score -= button.upgradeCost;
                            button.upgradeScoreValue();
                            button.upgradeCost *= 1.2f;
                        }
                    }
                    else
                    {
                        if (gameState.score < button.upgradeCost)
                        {
                            return SDL_APP_CONTINUE;
                        }
                        else if (gameState.score >= button.upgradeCost)
                        {
                            gameState.score -= button.upgradeCost;
                            button.upgradeScoreValue();
                            button.upgradeCost *= 1.2f;
                        }
                    }
                }
            }
        }

        if (isDragonUpgradeSidePanelVisible)
        {
            for (auto& dragonButton : gameState.dragonButtons)
            {
                if (mouseX >= dragonButton.getX() && mouseX <= dragonButton.getX() + dragonButton.getW() &&
                    mouseY >= dragonButton.getY() && mouseY <= dragonButton.getY() + dragonButton.getH())
                {
                    if (gameState.dragonCoins >= dragonButton.dragonUpgradeCost)
                    {
                        gameState.incrementScore *= dragonButton.multiplier;
                        gameState.dragonCoins -= dragonButton.dragonUpgradeCost;
                        dragonButton.dragonUpgradeCost *= 2.5f;
                    }
                }
            }
        }

        for (auto& dragon : gameState.dragons)
        {
            if (mouseX >= dragon.getX() && mouseX <= dragon.getX() + dragon.getW() &&
                mouseY >= dragon.getY() && mouseY <= dragon.getY() + dragon.getH())
            {
                if (dragon.health <= 0)
                {
                    dragon.health = dragon.baseHealth * 1.5f;
                    dragon.baseHealth = dragon.health;
                    gameState.dragonCoins += dragon.dragonSlayedPrice;
                    dragon.dragonSlayedPrice *= 1.25f;
                }

                gameState.score += gameState.incrementScore;
                dragon.health -= gameState.incrementScore;
                updateScoreText();
            }
        }

        updateScoreText();
        updateIncrementValueText();
        updateButtonText(renderer);
        updateDragonButtonText(renderer);
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    static Uint32 lastUpdateTime = SDL_GetTicks();
    Uint32 currentTime = SDL_GetTicks();

    float incrementValue = incrementValueCheck();

    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, backgroundImage, NULL, NULL);

    renderDragons(renderer, gameState);

    if (currentTime - lastUpdateTime >= 1000)
    {
        for (auto& button : gameState.buttons)
        {
            if (button.isActive())
            {
                button.incrementScore();
                lastUpdateTime = currentTime;
            }
        }

        for (auto& dragon : gameState.dragons)
        {
            dragon.health -= static_cast<long double>(incrementValue);
            renderHealthBar(dragon, renderer);
        }

        lastUpdateTime = currentTime;

        updateScoreText();
    }

    else
    {
        for (auto& dragon : gameState.dragons)
        {
            renderHealthBar(dragon, renderer);
        }
    }

    renderSidePanel(renderer, isSidePanelVisible, isDragonUpgradeSidePanelVisible);

    if (isSidePanelVisible)
    {
        renderButtons(renderer, gameState);
    }

    if (isDragonUpgradeSidePanelVisible)
    {
        renderDragonButtons(renderer, gameState);
    }

    upgradeToggleButton.render(renderer);
    dragonUpgradeToggleButton.render(renderer);

    renderHealthValueText(renderer, gameState);
    updateHealthValueText();
    renderScoreText(renderer, gameState);
    renderIncrementValueText(renderer, gameState);
    updateButtonText(renderer);
    renderDragonCoinText(renderer, gameState);
    updateDragonCoinText(renderer);
    updateDragonButtonText(renderer);

    if (!gameState.lastSaveTime.empty() && currentTime - lastSaveTime >= autoSaveInterval)
    {
        try
        {
            std::time_t lastSave = parseDateTime(gameState.lastSaveTime);
            std::time_t now = std::time(0);
            double timeDiffSeconds = difftime(now, lastSave);
            long double timeAFKHours = timeDiffSeconds / 3600.0f;

            long double afkBonus = std::min(1000.0L, static_cast<long double>(gameState.incrementScore * timeAFKHours * 0.25f));
            gameState.score += afkBonus;
            updateScoreText();
        }
        catch (const std::runtime_error& e)
        {
            std::cerr << "Error calculating AFK: " << e.what() << "\n";
        }

        saveGameState(gameState);
        lastSaveTime = currentTime;
        gameState.lastSaveTime = getCurrentDateTime();
    }

    else if (gameState.lastSaveTime.empty())
    {
        gameState.lastSaveTime = getCurrentDateTime();
    }

    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    if (font) TTF_CloseFont(font);
    if (texture) SDL_DestroyTexture(texture);
    if (backgroundImage) SDL_DestroyTexture(backgroundImage);
    if (dragon1) SDL_DestroyTexture(dragon1);
    if (incrementValueFont) TTF_CloseFont(incrementValueFont);
    if (HPValueFont) TTF_CloseFont(HPValueFont);
    if (upgradeValueFont) TTF_CloseFont(upgradeValueFont);

    TTF_Quit();
    SDL_Quit();
}