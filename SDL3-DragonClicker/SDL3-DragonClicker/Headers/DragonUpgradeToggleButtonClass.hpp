#pragma once
#include "includeHeaders.hpp"
#include "global.hpp"

class DragonUpgradeToggleButton
{
public:
    SDL_Texture* texture = nullptr;
    SDL_FRect textRect{};
    SDL_FRect buttonRect{};

    DragonUpgradeToggleButton()
    {
        buttonRect = { static_cast<float>(WINDOW_WIDTH) - 120, 7, 80, 80 };
    }

    ~DragonUpgradeToggleButton()
    {
        if (texture)
        {
            SDL_DestroyTexture(texture);
        }
    }

    void render(SDL_Renderer* renderer)
    {
        SDL_SetRenderDrawColor(renderer, 10, 10, 10, 0); // Niebieski kolor
        SDL_RenderFillRect(renderer, &buttonRect);

        if (shopSwordTexture)
        {
            SDL_FRect swordRect;
            swordRect.w = buttonRect.w * 1.0f; // Skala tekstury
            swordRect.h = buttonRect.h * 1.0f;
            swordRect.x = buttonRect.x + (buttonRect.w - swordRect.w) / 2;
            swordRect.y = buttonRect.y + (buttonRect.h - swordRect.h) / 2;

            SDL_RenderTexture(renderer, shopSwordTexture, nullptr, &swordRect);
        }
    }

    bool isClicked(float mouseX, float mouseY)
    {
        return mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w &&
            mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h;
    }
};