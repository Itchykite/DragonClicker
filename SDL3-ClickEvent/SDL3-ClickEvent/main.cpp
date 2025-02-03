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
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <string>

 /* We will use this renderer to draw into this window every frame. */
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static TTF_Font* font = NULL;

static int rx = 100;
static int ry = 100;
static int rw = 400;
static int rh = 400;

int clickCount{};

void renderText(const std::string& text, int x, int y, SDL_Color* color)
{
    // Tworzymy powierzchniê z tekstem
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), *color);
    if (!textSurface)
    {
        SDL_Log("Unable to create text surface! SDL_ttf Error: %s", SDL_GetError());
        return;
    }

    // Tworzymy teksturê z powierzchni
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture)
    {
        SDL_Log("Unable to create texture from text surface! SDL Error: %s", SDL_GetError());
        SDL_DestroySurface(textSurface);
        return;
    }

    // Rysowanie tekstu na ekranie
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_Rect renderQuad = { x, y, textWidth, textHeight };
    SDL_RenderCopyEx(renderer, textTexture, NULL, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);

    // Zwolnienie zasobów
    SDL_DestroyTexture(textTexture);
    SDL_DestroySurface(textSurface);
}


bool checkMouseClickonRect(int mouseX, int mouseY, int objX, int objY, int objWidth, int objHeight)
{
    return (mouseX >= objX && mouseX <= objX + objWidth && mouseY >= objY && mouseY <= objY + objHeight);
}

void game() // game instance
{
    std::string ns = std::to_string(clickCount);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_FRect rect(static_cast<float>(rx), static_cast<float>(ry), static_cast<float>(rw), static_cast<float>(rh));

    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderDebugTextFormat(renderer, 300, 50, ns.c_str());

    SDL_RenderPresent(renderer);
}

void endOfGame() // end of the game, if you can't tell XD
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_RenderDebugTextFormat(renderer, 300, 300, "Koniec");

    SDL_RenderPresent(renderer);
}

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO)) 
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", 600, 600, 0, &window, &renderer)) 
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (TTF_Init() == -1) 
    {
        SDL_Log("SDL_ttf could not initialize! SDL_ttf Error: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // £adowanie czcionki
    font = TTF_OpenFont("C:\\Users\\cyran\\Downloads\\Poppins\\Poppins-Bold.ttf", 24); // Podaj œcie¿kê do czcionki TTF
    if (font == NULL) 
    {
        SDL_Log("Failed to load font! SDL_ttf Error: %s", SDL_GetError());
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

        if (mouseX >= rx && mouseX <= rx + rw && mouseY >= ry && mouseY <= ry + rh)
        {
            clickCount++;
        }

        if (clickCount == 10)
        {
            endOfGame();

            SDL_Delay(3000);

            return SDL_APP_SUCCESS;
        }
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    game(); // game instance

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}