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
SDL_FRect upgradeButtonField;
SDL_FRect upgradeButtonFieldTwo;

long double score = {0.00};
std::string s_score = std::to_string(score);

class UpgradeButtons
{
public:
    UpgradeButtons(float& upgradeCost, float& incrementValue, Uint32 lastUpdateTime = 0, bool isUpgrading = false) : upgradeCost(upgradeCost), incrementValue(incrementValue), lastUpdateTime(lastUpdateTime), isUpgrading(isUpgrading)
    {
        
    }

    void upgradeButtonFun()
    {
        float clickFieldSizePercentage = 0.25f; // wielkoœæ pola jako procent ekranu
        float squareSize = static_cast<int>(std::min(WINDOW_WIDTH, WINDOW_HEIGHT) * clickFieldSizePercentage); // wielkoœæ kwadratu
        int margin = 20; // odstêp od prawej krawêdzi

        float rectangleHeight = squareSize / 4.0f; // wysokoœæ prostok¹ta jest æwiartk¹ szerokoœci

        upgradeButtonField =
        {
            WINDOW_WIDTH - squareSize - margin,  // x
            (WINDOW_HEIGHT - rectangleHeight) / 2.0f,  // y (pozycja na œrodku)
            squareSize,  // szerokoœæ
            rectangleHeight  // wysokoœæ
        };

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &upgradeButtonField);
    }

private:
    bool isUpgrading = false;
    Uint32 lastUpdateTime = 0;
    float incrementValue = 0.01f;  // Pocz¹tkowa wartoœæ do dodania
    float upgradeCost = 0;
};

namespace upgradeButton
{
    bool isUpgrading = false;
    Uint32 lastUpdateTime = 0;
    float incrementValue = 0.01f;  // Pocz¹tkowa wartoœæ do dodania
    float upgradeCost = 50;
}

namespace upgradeButtonTwo
{
    bool isUpgrading = false;
    Uint32 lastUpdateTime = 0;
    float incrementValue = 0.05f;  // Pocz¹tkowa wartoœæ do dodania
    float upgradeCost = 100;
}

float incrementValueCheck()
{
    // Overall Increment Value
    float incrementValue = 0.0f;

    if (upgradeButton::isUpgrading && upgradeButtonTwo::isUpgrading)
    {
        incrementValue = upgradeButton::incrementValue + upgradeButtonTwo::incrementValue;
    }

    else if (upgradeButton::isUpgrading)
    {
        incrementValue = upgradeButton::incrementValue;
    }

    else if (upgradeButtonTwo::isUpgrading)
    {
        incrementValue = upgradeButtonTwo::incrementValue;
    }

    return incrementValue;
}

void upgradeButtonFun()
{
    float clickFieldSizePercentage = 0.25f; // wielkoœæ pola jako procent ekranu
    float squareSize = static_cast<int>(std::min(WINDOW_WIDTH, WINDOW_HEIGHT) * clickFieldSizePercentage); // wielkoœæ kwadratu
    int margin = 20; // odstêp od prawej krawêdzi

    float rectangleHeight = squareSize / 4.0f; // wysokoœæ prostok¹ta jest æwiartk¹ szerokoœci

    upgradeButtonField =
    {
        WINDOW_WIDTH - squareSize - margin,  // x
        (WINDOW_HEIGHT - rectangleHeight) / 2.0f,  // y (pozycja na œrodku)
        squareSize,  // szerokoœæ
        rectangleHeight  // wysokoœæ
    };

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &upgradeButtonField);
}

void upgradeButtonFunTwo()
{
    float clickFieldSizePercentage = 0.25f; // wielkoœæ pola jako procent ekranu
    float squareSize = static_cast<int>(std::min(WINDOW_WIDTH, WINDOW_HEIGHT) * clickFieldSizePercentage); // wielkoœæ kwadratu
    int margin = 20; // odstêp od prawej krawêdzi

    float rectangleHeight = squareSize / 4.0f; // wysokoœæ prostok¹ta jest æwiartk¹ szerokoœci

    // Pozycja drugiego prostok¹ta, uwzglêdniaj¹c odstêp od pierwszego
    float yPosition = (WINDOW_HEIGHT - rectangleHeight) / 2.0f + rectangleHeight + 10.0f;  // odstêp 10px miêdzy prostok¹tami

    upgradeButtonFieldTwo =
    {
        WINDOW_WIDTH - squareSize - margin,  // x
        yPosition,  // y (pozycja poni¿ej pierwszego prostok¹ta)
        squareSize,  // szerokoœæ
        rectangleHeight  // wysokoœæ
    };

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &upgradeButtonFieldTwo);
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
        int mouseX = event->button.x;
        int mouseY = event->button.y;

        if (mouseX >= clickField.x && mouseX <= clickField.x + clickField.w &&
            mouseY >= clickField.y && mouseY <= clickField.y + clickField.h)
        {
            score++;  // zwiêkszenie wyniku po klikniêciu w obrêb kwadratu
        }

        if (mouseX >= upgradeButtonField.x && mouseX <= upgradeButtonField.x + upgradeButtonField.w &&
            mouseY >= upgradeButtonField.y && mouseY <= upgradeButtonField.y + upgradeButtonField.h)
        {
            // Jeœli ulepszanie jeszcze siê nie zaczê³o, to je uruchamiamy
            if (!upgradeButton::isUpgrading)
            {
                upgradeButton::isUpgrading = true;  // Ustawiamy flagê na true
                upgradeButton::lastUpdateTime = SDL_GetTicks();  // Resetujemy czas
            }
            else
            {
                if (score < upgradeButton::upgradeCost)
                {
                    return SDL_APP_CONTINUE;  /* carry on with the program! */
                }

                else
                {
                    score -= upgradeButton::upgradeCost;
                    upgradeButton::incrementValue += 0.01f;  // Zwiêkszamy wartoœæ dodawan¹ do wyniku o 0.01 przy ka¿dym klikniêciu
                    upgradeButton::upgradeCost += (upgradeButton::upgradeCost * 0.2f);

                    // Debug
                    std::cout << "1 - Cost: " << upgradeButton::upgradeCost << " Increment value: " << upgradeButton::incrementValue << "\n";
                }
            }
        }

        if (mouseX >= upgradeButtonFieldTwo.x && mouseX <= upgradeButtonFieldTwo.x + upgradeButtonFieldTwo.w &&
            mouseY >= upgradeButtonFieldTwo.y && mouseY <= upgradeButtonFieldTwo.y + upgradeButtonFieldTwo.y)
        {
            if (!upgradeButtonTwo::isUpgrading)
            {
                upgradeButtonTwo::isUpgrading = true;  // Ustawiamy flagê na true
                upgradeButtonTwo::lastUpdateTime = SDL_GetTicks();  // Resetujemy czas
            }
            else
            {
                if (score < upgradeButtonTwo::upgradeCost)
                {
                    return SDL_APP_CONTINUE;  /* carry on with the program! */
                }

                else
                {
                    score -= upgradeButtonTwo::upgradeCost;
                    upgradeButtonTwo::incrementValue += 0.01f;  // Zwiêkszamy wartoœæ dodawan¹ do wyniku o 0.01 przy ka¿dym klikniêciu
                    upgradeButtonTwo::upgradeCost += (upgradeButtonTwo::upgradeCost * 0.2f);

                    // Debug
                    std::cout << "2 - Cost: " << upgradeButtonTwo::upgradeCost << " Increment value: " << upgradeButtonTwo::incrementValue << "\n";
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
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // t³o
    SDL_RenderClear(renderer);

    renderClickField();
    renderScoreText();
    renderIncrementValueText();

    renderClickField(); // renderuje w oknie pole do klikniêcia

    if (upgradeButton::isUpgrading)
    {
        Uint32 currentTime = SDL_GetTicks();

        // Zwiêkszamy wynik co sekundê na podstawie incrementValue
        if (currentTime - upgradeButton::lastUpdateTime >= 1000)  // Jeœli minê³a 1 sekunda
        {
            score += upgradeButton::incrementValue;  // Zwiêkszamy wynik o wartoœæ incrementValue
            upgradeButton::lastUpdateTime = currentTime;  // Aktualizujemy czas
        }

        updateScoreText();
    }

    if (upgradeButtonTwo::isUpgrading)
    {
        Uint32 currentTime = SDL_GetTicks();

        // Zwiêkszamy wynik co sekundê na podstawie incrementValue
        if (currentTime - upgradeButtonTwo::lastUpdateTime >= 1000)  // Jeœli minê³a 1 sekunda
        {
            score += upgradeButtonTwo::incrementValue;  // Zwiêkszamy wynik o wartoœæ incrementValue
            upgradeButtonTwo::lastUpdateTime = currentTime;  // Aktualizujemy czas
        }

        updateScoreText();
    }

    upgradeButtonFun();
    upgradeButtonFunTwo();

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
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