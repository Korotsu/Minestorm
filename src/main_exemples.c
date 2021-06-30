
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>

int lol(int argc, char* argv[])
{
    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("SDL_Init error: %s", SDL_GetError());
        return 1;
    }
    atexit(SDL_Quit); // Tell the program to call SDL_Quit() when it exits

    // Create SDL window
    SDL_Window* window = SDL_CreateWindow("Test SDL 2.0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        SDL_Log("SDL_CreateWindow error: %s", SDL_GetError());
        return 1;
    }

    // Create SDL renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        SDL_Log("SDL_CreateRenderer error: %s", SDL_GetError());
        return 1;
    }

    // Some vars
    Uint32 time = 0;
    bool running = true;
    float moving_position = 0.f;

    // Main loop
    bool moving = true;
    while (running)
    {
        // Init delta time
        Uint32 current_time = SDL_GetTicks();
        float delta_time = (time > 0) ? ((current_time - time) / 1000.f) : 1.0f / 60.0f;
        time = current_time;

        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                // Triggered when user close the app
                case SDL_QUIT:
                    running = false;
                    break;

                // Triggered on keyboard key down
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        running = false;
                    else if (event.key.keysym.sym == SDLK_SPACE)
                        moving = !moving;
                    break;

                default:
                    break;
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0x88, 0x66, 0xBB, 0xFF);
        SDL_RenderClear(renderer);

        //============================
        // Start drawing with renderer
        SDL_SetRenderDrawColor(renderer, 0x0F, 0xFF, 0xFF, 0xFF);
        int x = 64;
        // FillRect
        {
            if (moving)
                moving_position += 20.f * delta_time;
            SDL_Rect rect = { x + moving_position, 64, 64, 64 };
            SDL_RenderFillRect(renderer, &rect);
        }

        x += 128;
        // DrawRect
        {
            SDL_Rect rect = { x, 64, 64, 64 };
            SDL_RenderDrawRect(renderer, &rect);
        }

        x += 128;
        // DrawLine
        {
            SDL_RenderDrawLine(renderer, x +  0, 64, x + 64, 128);
            SDL_RenderDrawLine(renderer, x + 64, 64, x +  0, 128);
        }

        x += 128;
        // DrawPoint
        {
            SDL_RenderDrawPoint(renderer, x +  0,  64);
            SDL_RenderDrawPoint(renderer, x + 64,  64);
            SDL_RenderDrawPoint(renderer, x + 64, 128);
            SDL_RenderDrawPoint(renderer, x +  0, 128);
        }

        //============================
        // Update screen
        SDL_RenderPresent(renderer);

        // Small delay to let the CPU breath
        SDL_Delay(1);
    }

    SDL_DestroyWindow(window);
    return 0;
}

