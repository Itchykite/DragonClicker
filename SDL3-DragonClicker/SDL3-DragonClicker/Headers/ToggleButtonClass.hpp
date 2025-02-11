#pragma once
#include "includeHeaders.hpp"
#include "global.hpp"
#include "SDL3/SDL.h"

class ToggleButton
{
public:
    SDL_Texture* texture = nullptr;
    SDL_FRect textRect{};
    SDL_FRect buttonRect{};

    ToggleButton()
    {
        buttonRect = { static_cast<float>(WINDOW_WIDTH) - 200, 10, 65, 65 };
    }

    ~ToggleButton()
    {
        if (texture)
        {
            SDL_DestroyTexture(texture);
        }
    }

    void render(SDL_Renderer* renderer)
    {
        SDL_SetRenderDrawColor(renderer, 10, 10, 10, 0);
        SDL_RenderFillRect(renderer, &buttonRect);

        if (shopTexture)
        {
            SDL_FRect shopRect;
            shopRect.w = buttonRect.w * 1.0f; // Skala tekstury
            shopRect.h = buttonRect.h * 1.0f;
            shopRect.x = buttonRect.x + (buttonRect.w - shopRect.w) / 2;
            shopRect.y = buttonRect.y + (buttonRect.h - shopRect.h) / 2;

            SDL_RenderTexture(renderer, shopTexture, nullptr, &shopRect);
        }
    }

    bool isClicked(float mouseX, float mouseY)
    {
        return mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w &&
            mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h;
    }
};