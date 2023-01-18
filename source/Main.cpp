#include <SDL.h>
#include <ciso646>
#include <iostream>
#include "Scene.h"
#include "Render_System.h"
#include "Audio_System.h"
#include "Physics_System.h"
int main(int, char**)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return 1;
    }
    // Create a window
    SDL_Window* window = SDL_CreateWindow("My Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);
    if (!window) {
        std::cerr << "Error creating window: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create a scene
    blood_engine::Scene scene(*window);

    // Prepare the Systems
    // Render
    std::unique_ptr<blood_engine::Render_System> render_system =
        std::unique_ptr<blood_engine::Render_System>();
    // Physics
    std::unique_ptr<blood_engine::Physics_System> physics_system =
        std::unique_ptr<blood_engine::Physics_System>();
    // Audio
    std::unique_ptr<blood_engine::Audio_System> audio_system =
        std::unique_ptr<blood_engine::Audio_System>();

    scene.AddSystem(std::move(render_system));
    scene.AddSystem(std::move(physics_system));
    scene.AddSystem(std::move(audio_system));
	
    // Create a new Kernel
    blood_engine::Kernel kernel;
	
    // Run the kernel
    kernel.Run(&scene);
	
    // Main loop
    bool running = true;
    while (running) {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Update the scene
        scene.Update();
        
        // Render the scene
        scene.Render();

        // Swap buffers
        SDL_GL_SwapWindow(window);
    }

    // Clean up
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
