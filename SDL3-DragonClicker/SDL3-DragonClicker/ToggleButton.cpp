#include "ToggleButton.hpp"

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern TTF_Font* font;

ToggleButton::ToggleButton()
{
    buttonRect = { static_cast<float>(WINDOW_WIDTH) - 100, 50, 40, 40 };
    texture = nullptr;
}

ToggleButton::~ToggleButton()
{
    if (texture)
    {
        SDL_DestroyTexture(texture);
    }
}

void ToggleButton::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
    SDL_RenderFillRect(renderer, &buttonRect);

    if (texture)
    {
        textRect.x = buttonRect.x + (buttonRect.w - textRect.w) / 2;
        textRect.y = buttonRect.y + (buttonRect.h - textRect.h) / 2;
        SDL_RenderTexture(renderer, texture, nullptr, &textRect);
    }
}

bool ToggleButton::isClicked(float mouseX, float mouseY)
{
    return mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w &&
        mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h;
}

DragonUpgradeToggleButton::DragonUpgradeToggleButton()
{
    buttonRect = { static_cast<float>(WINDOW_WIDTH) - 50, 50, 40, 40 };
    texture = nullptr;
}

DragonUpgradeToggleButton::~DragonUpgradeToggleButton()
{
    if (texture)
    {
        SDL_DestroyTexture(texture);
    }
}

void DragonUpgradeToggleButton::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
    SDL_RenderFillRect(renderer, &buttonRect);

    if (texture)
    {
        textRect.x = buttonRect.x + (buttonRect.w - textRect.w) / 2;
        textRect.y = buttonRect.y + (buttonRect.h - textRect.h) / 2;
        SDL_RenderTexture(renderer, texture, nullptr, &textRect);
    }
}

bool DragonUpgradeToggleButton::isClicked(float mouseX, float mouseY)
{
    return mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w &&
        mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h;
}