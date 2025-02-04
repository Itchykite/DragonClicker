/* clear.c ... */

/*
 * This example code creates an SDL window and renderer, and then clears the
 * window to a different color every frame, so you'll effectively get a window
 * that's smoothly fading between colors.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_main.h>

#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>

// Rozmiar okna
static int WINDOW_WIDTH = 1920;
static int WINDOW_HEIGHT = 1080;

 /* We will use this renderer to draw into this window every frame. */
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* texture = NULL;
static TTF_Font* font = NULL;
TTF_Font* incrementValueFont = NULL;
SDL_Texture* incrementValueTexture = NULL;

SDL_FRect clickField;

long double score = {0.00};
long int incrementScore = 1;
std::string s_score = std::to_string(score);
Uint32 lastIncrementTime = 0; // Czas ostatniej aktualizacji

class UpgradeButton
{
public:
    bool isObjectUpgrading = false;
    float incrementValue;
    float upgradeCost;
    static Uint32 lastUpdateTime;

    UpgradeButton(float upgradeCost, float incrementValue, float incrementValueChange)
        : upgradeCost(upgradeCost), incrementValue(incrementValue), incrementValueChange(incrementValueChange), isObjectUpgrading(false)
    {
        float clickFieldSizePercentage = 0.25f;
        float squareSize = static_cast<float>(std::min(WINDOW_WIDTH, WINDOW_HEIGHT) * clickFieldSizePercentage);
        int margin = 20;
        float rectangleHeight = squareSize / 4;

        // Automatyczne pozycjonowanie przycisków jeden pod drugim
        upgradeButtonField = 
        {
            WINDOW_WIDTH - squareSize - margin,
            static_cast<float>(yOffset),
            squareSize,
            rectangleHeight
        };

        // Zwiêkszenie pozycji dla kolejnego przycisku
        yOffset += rectangleHeight + 10;
    }

    void render(SDL_Renderer* renderer)
    {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &upgradeButtonField);
    }

    void incrementScore()
    {
        if (isObjectUpgrading)
        {
            score += incrementValue;
        }
    }

    void upgradeScoreValue()
    {
        incrementValue += incrementValueChange;
    }

    float getX() const { return upgradeButtonField.x; }
    float getY() const { return upgradeButtonField.y; }
    float getW() const { return upgradeButtonField.w; }
    float getH() const { return upgradeButtonField.h; }

    void activate() { isObjectUpgrading = true; }
    void deactivate() { isObjectUpgrading = false; }
    bool isActive() const { return isObjectUpgrading; }
    float getIncrementValue() const { return incrementValue; }

private:
    static float yOffset; // Przechowuje przesuniêcie w osi Y dla kolejnych przycisków
    SDL_FRect upgradeButtonField{};
    float incrementValueChange;
};

std::vector<UpgradeButton> buttons;
float UpgradeButton::yOffset = 100.0f;
Uint32 UpgradeButton::lastUpdateTime = 0;

void createButtons()
{
    buttons.emplace_back(50, 0.0f, 0.1f);
    buttons.emplace_back(100, 0.0f, 0.5f);
    buttons.emplace_back(200, 0.0f, 1.0f);
}

void renderButtons(SDL_Renderer* renderer)
{
    for (auto& button : buttons)
    {
        button.render(renderer);
    }
}

float incrementValueCheck()
{
    float incrementValue = 0.0f;

    for (const auto& button : buttons)
    {
        if (button.isActive())
        {
            incrementValue += button.getIncrementValue();
        }
    }

    return incrementValue;
}

void updateScoreText()
{
    if (texture)
    {
        SDL_DestroyTexture(texture); // Usuwamy star¹ teksturê
        texture = NULL;
    }

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2) << score;
    s_score = stream.str();

    SDL_Color color = { 0, 0, 0, 255 };
    SDL_Surface* text = TTF_RenderText_Blended(font, s_score.c_str(), 0 ,color);

    if (text)
    {
        texture = SDL_CreateTextureFromSurface(renderer, text);
        SDL_DestroySurface(text);
    }
}

void updateIncrementValueText()
{
    float incrementValue = incrementValueCheck();

    if (incrementValueTexture)
    {
        SDL_DestroyTexture(incrementValueTexture); // Usuwamy star¹ teksturê
        incrementValueTexture = NULL;
    }

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2) << incrementValue;
    std::string incrementValueS = stream.str();

    SDL_Color color = { 0, 0, 0, 255 };
    SDL_Surface* text = TTF_RenderText_Blended(incrementValueFont, incrementValueS.c_str(), 0 ,color);

    if (text)
    {
        incrementValueTexture = SDL_CreateTextureFromSurface(renderer, text);
        SDL_DestroySurface(text);
    }
}

void renderScoreText()
{
    if (!texture) return;

    float w = 0, h = 0;
    SDL_GetTextureSize(texture, &w, &h);

    SDL_FRect dst;
    dst.w = w;
    dst.h = h;
    dst.x = (WINDOW_WIDTH - w) / 2; // centrowanie
    dst.y = 50;

    SDL_RenderTexture(renderer, texture, NULL, &dst);
}

void renderIncrementValueText()
{
    if (!texture) return;

    float w = 0, h = 0;
    SDL_GetTextureSize(incrementValueTexture, &w, &h);

    SDL_FRect dst;
    dst.w = w;
    dst.h = h;
    dst.x = (WINDOW_WIDTH - w) / 2; // Centrowanie
    dst.y = 100;  // Poni¿ej g³ównego tekstu, zmniejszaj¹c y

    SDL_RenderTexture(renderer, incrementValueTexture, NULL, &dst);
}

void renderClickField()
{
    float clickFieldSizePercentage = 0.75f; // wielkoœæ pola
    float squareSize = std::min(WINDOW_WIDTH, WINDOW_HEIGHT) * clickFieldSizePercentage; // obliczanie wielkoœci wzglêdem ekranu (float)
    clickField = { (WINDOW_WIDTH - squareSize) / 2.0f, (WINDOW_HEIGHT - squareSize) / 2.0f, squareSize, squareSize }; // pozycja jako float

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &clickField);
}

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    //SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

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

    if (!TTF_Init()) 
    {
        SDL_Log("Couldn't initialise SDL_ttf: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    font = TTF_OpenFont("Poppins-Bold.ttf", 24.0f);
    if (!font)
    {
        SDL_Log("Couldn't open font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    incrementValueFont = TTF_OpenFont("Poppins-Bold.ttf", 16.0f);
    if (!incrementValueFont)
    {
        SDL_Log("Couldn't open incrementValue font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    text = TTF_RenderText_Blended(font, s_score.c_str(), 10, color);
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

    updateScoreText();
    createButtons();

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        if (mouseX >= clickField.x && mouseX <= clickField.x + clickField.w &&
            mouseY >= clickField.y && mouseY <= clickField.y + clickField.h)
        {
            score += incrementScore;
            updateScoreText();
        }

        for (auto& button : buttons)
        {
            if (mouseX >= button.getX() && mouseX <= button.getX() + button.getW() &&
                mouseY >= button.getY() && mouseY <= button.getY() + button.getH())
            {
                if (!button.isObjectUpgrading)
                {
                    button.isObjectUpgrading = true;
                    UpgradeButton::lastUpdateTime = SDL_GetTicks(); // Resetujemy czas
                    std::cout << "Ulepszanie aktywowane!" << std::endl;
                }
                else
                {
                    if (score < button.upgradeCost)
                    {
                        return SDL_APP_CONTINUE;  /* carry on with the program! */
                    }
                    else if (score >= button.upgradeCost)
                    {
                        // Jeœli mamy wystarczaj¹co punktów na upgrade
                        score -= button.upgradeCost;
                        button.upgradeScoreValue(); // Zwiêkszamy incrementValue
                        button.upgradeCost *= 1.2f;  // Zwiêkszamy koszt ulepszania
                        std::cout << "1 - Cost: " << button.upgradeCost << " Increment value: " << button.getIncrementValue() << "\n";
                    }
                }
            }
        }

        updateScoreText();
        updateIncrementValueText();
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}


/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    static Uint32 lastUpdateTime = SDL_GetTicks();

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    renderClickField();
    renderScoreText();
    renderIncrementValueText();
    renderButtons(renderer);

    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastUpdateTime >= 1000)
    {
        for (auto& button : buttons)
        {
            if (button.isActive())
            {
                std::cout << "Before increment: score = " << score << ", incrementValue = " << button.getIncrementValue() << std::endl;
                button.incrementScore(); // Zwiêkszamy wynik
                std::cout << "After increment: score = " << score << std::endl;
                lastUpdateTime = currentTime;
            }
        }

        updateScoreText();
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

    TTF_Quit();
    SDL_Quit();
}