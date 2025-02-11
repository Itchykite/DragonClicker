#include "Headers/includeHeaders.hpp"
#include "Headers/includeFunctions.hpp"
#include "Headers/global.hpp"
#include "Headers/headers.hpp"

std::string getCurrentDateTime()
{
    auto now = std::chrono::system_clock::now();
    std::time_t in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%FT%T%z");

    return ss.str();
}

std::time_t parseDateTime(const std::string& dateTimeStr)
{
    std::tm t{};
    std::istringstream ss(dateTimeStr);

    ss >> std::get_time(&t, "%FT%T%z");
    if (ss.fail())
    {
        throw std::runtime_error("Err");
    }

    return mktime(&t);
}

void renderSidePanel(SDL_Renderer* renderer)
{
    if (isSidePanelVisible)
    {
        SDL_FRect sidePanel = { WINDOW_WIDTH - 300, 0, 300, WINDOW_HEIGHT };
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 128); // Szary kolor
        SDL_RenderFillRect(renderer, &sidePanel);
    }

    if (isDragonUpgradeSidePanelVisible)
    {
        SDL_FRect sidePanel = { WINDOW_WIDTH - 300, 0, 300, WINDOW_HEIGHT };
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 128); // Szary kolor
        SDL_RenderFillRect(renderer, &sidePanel);
    }
}

void updateToggleButtonText(SDL_Renderer* renderer)
{
    if (upgradeToggleButton.texture)
    {
        SDL_DestroyTexture(upgradeToggleButton.texture);
        upgradeToggleButton.texture = nullptr;
    }

    SDL_Color color = { 255, 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, "Toggle", 0, color);
    if (textSurface)
    {
        upgradeToggleButton.texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        upgradeToggleButton.textRect.w = textSurface->w;
        upgradeToggleButton.textRect.h = textSurface->h;
        SDL_DestroySurface(textSurface);
    }
}

void updateDragonUpgradeToggleButtonText(SDL_Renderer* renderer)
{
    if (dragonUpgradeToggleButton.texture)
    {
        SDL_DestroyTexture(dragonUpgradeToggleButton.texture);
        dragonUpgradeToggleButton.texture = nullptr;
    }

    SDL_Color color = { 10, 10, 10, SDL_ALPHA_OPAQUE };
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, "Toggle", 0, color);
    if (textSurface)
    {
        dragonUpgradeToggleButton.texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        dragonUpgradeToggleButton.textRect.w = textSurface->w;
        dragonUpgradeToggleButton.textRect.h = textSurface->h;
        SDL_DestroySurface(textSurface);
    }
}

/* This function runs once at startup. */
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

    font = TTF_OpenFont("./Src/Font/EagleLake-Regular.ttf", 32.0f);
    if (!font)
    {
        SDL_Log("Couldn't open font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    incrementValueFont = TTF_OpenFont("./Src/Font/EagleLake-Regular.ttf", 16.0f);
    if (!incrementValueFont)
    {
        SDL_Log("Couldn't open incrementValue font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    HPValueFont = TTF_OpenFont("./Src/Font/EagleLake-Regular.ttf", 24.0f);
    if (!HPValueFont)
    {
        SDL_Log("Couldn't open incrementValue font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    upgradeValueFont = TTF_OpenFont("./Src/Font/EagleLake-Regular.ttf", 20.0f);
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

    backgroundImage = IMG_LoadTexture(renderer, "./Src/Images/background.jpeg");
    dragon1 = IMG_LoadTexture(renderer, "./Src/Images/dragon1.png");
    dragonCoin = IMG_LoadTexture(renderer, "./Src/Images/coin.png");
	shopTexture = IMG_LoadTexture(renderer, "./Src/Images/pouch.png");
    shopSwordTexture = IMG_LoadTexture(renderer, "./Src/Images/swordshop.png");

    if (!backgroundImage || !dragon1 || !dragonCoin || !shopTexture || !shopSwordTexture)
    {
        std::cerr << "B³¹d wczytywania obrazu: " << SDL_GetError() << std::endl;
        return SDL_APP_FAILURE;
    }

    createDragons();
    createButtons();
    createDragonButtons();

    updateScoreText();
    updateIncrementValueText();
    updateButtonText(renderer);
    updateDragonButtonText(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        saveGameState(gameState);
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
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
                        button.upgradeScoreValue(); // Zwiêkszamy incrementValue
                        button.upgradeCost *= 1.2f;  // Zwiêkszamy koszt ulepszania
                    }
                }
                else
                {
                    if (gameState.score < button.upgradeCost)
                    {
                        return SDL_APP_CONTINUE;  /* carry on with the program! */
                    }

                    else if (gameState.score >= button.upgradeCost)
                    {
                        gameState.score -= button.upgradeCost;
                        button.upgradeScoreValue(); // Zwiêkszamy incrementValue
                        button.upgradeCost *= 1.2f;  // Zwiêkszamy koszt ulepszania
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

        updateScoreText();
        updateIncrementValueText();
        updateButtonText(renderer); // Dodaj wywo³anie funkcji aktualizuj¹cej teksty przycisków
        updateDragonButtonText(renderer); // Dodaj wywo³anie funkcji aktualizuj¹cej teksty przycisków
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    static Uint32 lastUpdateTime = SDL_GetTicks();
    Uint32 currentTime = SDL_GetTicks();

    float incrementValue = incrementValueCheck();

    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, backgroundImage, NULL, NULL);

    renderDragons(renderer);

    if (currentTime - lastUpdateTime >= 1000)
    {
        for (auto& button : gameState.buttons)
        {
            if (button.isActive())
            {
                button.incrementScore(); // Zwiêkszamy wynik
                lastUpdateTime = currentTime;
            }
        }

        for (auto& dragon : gameState.dragons)
        {
            std::cout << "Dragon's health: " << dragon.health << "\n";
            dragon.health -= static_cast<long double>(incrementValue);
            renderHealthBar(dragon, renderer); // Renderuj pasek 
        }

        lastUpdateTime = currentTime;

        updateScoreText();
    }

    else
    {
        for (auto& dragon : gameState.dragons)
        {
            renderHealthBar(dragon, renderer); // Renderuj pasek 
        }
    }

    renderSidePanel(renderer);
    renderButtons(renderer);
    renderDragonButtons(renderer);
    upgradeToggleButton.render(renderer);
    dragonUpgradeToggleButton.render(renderer);

    renderHealthValueText();
    updateHealthValueText();
    renderScoreText();
    renderIncrementValueText();
    updateButtonText(renderer);
    renderDragonCoinText();
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
            std::cerr << "Error calulating AFK: " << e.what() << "\n";
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

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    if (font)
    {
        TTF_CloseFont(font);
    }

    if (texture)
    {
        SDL_DestroyTexture(texture);
    }

    if (backgroundImage)
    {
        SDL_DestroyTexture(backgroundImage);
    }

    if (dragon1)
    {
        SDL_DestroyTexture(dragon1);
    }

    TTF_Quit();
    SDL_Quit();
}