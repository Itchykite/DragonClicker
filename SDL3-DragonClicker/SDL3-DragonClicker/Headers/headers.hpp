#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <nlohmann/json.hpp>

#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <ctime>
#include <chrono>