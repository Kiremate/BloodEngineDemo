/**
 * @author Bloodmask
 * @copyright Copyright (c) 2023, Bloodmask
 * @license All rights reserved
 */
#include <SDL.h>
#include <ciso646>
#include <iostream>
#include "Scene.h"
#include "Kernel.h"
#include "Render_System.h"
#include "Audio_System.h"
#include "Physics_System.h"
#include "Input_System.h"
#include <Cube.hpp>

int main(int, char**)
{
	const int RESOLUTION_WIDTH = 800;
    const int RESOLUTION_HEIGHT = 600;

    SDL_GLContext gl_context;

    bool fullScreen = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return 1;
    }
    // Create a window
    SDL_Window* window = SDL_CreateWindow("Bloodmask", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Error creating window: " << SDL_GetError() << std::endl;
        return 1;
    }
    if (window)
    {
        SDL_GLContext gl_context = SDL_GL_CreateContext(window);

        assert(gl_context != nullptr);

        if (gl_context && glt::initialize_opengl_extensions())
        {
            if (fullScreen)
            {
                SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            }
        }
    }

    // Create a scene
	blood_engine::Scene scene(window, &gl_context);
    // Prepare the models
    std::shared_ptr<Drawable> cubeDrawable = std::shared_ptr<Drawable>(new Cube);
    std::shared_ptr<Material> defaultMaterial = Material::default_material();
    std::shared_ptr<Model> cube = std::make_shared<Model>();
    cube->add(cubeDrawable, defaultMaterial);
    // Prepare the Systems
    // Input System
	std::shared_ptr<blood_engine::Input_System> input_system = std::make_shared<blood_engine::Input_System>();
    input_system->SetScene(std::make_shared<blood_engine::Scene>(window, &gl_context));
    scene.AddSystem(input_system);
    // Render System
    std::shared_ptr<blood_engine::Render_System> render_system = std::make_shared<blood_engine::Render_System>();
    render_system->SetResolution(RESOLUTION_WIDTH, RESOLUTION_HEIGHT);
    render_system->CreateRenderComponent("Cube", cube);
    render_system->SetScene(std::make_shared<blood_engine::Scene>(window, &gl_context));
    scene.AddSystem(render_system);
    // Physics System
    std::shared_ptr<blood_engine::Physics_System> physics_system = std::make_shared<blood_engine::Physics_System>();
    scene.AddSystem(physics_system);
    // Audio System
    std::shared_ptr<blood_engine::Audio_System> audio_system = std::make_shared<blood_engine::Audio_System>();
    scene.AddSystem(audio_system);
	
    // Create a new Kernel
    blood_engine::Kernel kernel;
    kernel.Initialize();
    // Run the kernel
    kernel.Run(&scene);
	
    // Clean up
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
