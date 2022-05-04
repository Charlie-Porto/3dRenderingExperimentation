#ifndef sim_manager_cpp
#define sim_manager_cpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <vector>

/*
    --------------- Purpose ---------------
    
    This class is hella important for running programs using SDL2, broh.
    It contains most of the code necessary for getting an SDL2 program up and running, 
    and must be used in combination with a main file. 
*/

class Simulation 
{       
    public:
        Simulation(){};
        ~Simulation(){};
        
        void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
        
        /* Events */
        void handleEvents();
        void clear_frame_events();

        void update();
        void render();
        void clearRenderer();
        void clean();
        bool running() { return isRunning; }
        
        
        static SDL_Renderer* renderer;
        static SDL_Event event;
        static std::vector<SDL_Event> frame_events;
        static bool isRunning;

    private:
        int cnt = 0;
        SDL_Window *window;
        
};

/*--------------------------- Activate Stuff -------------------- */
/* set up renderer */
SDL_Renderer* Simulation::renderer = nullptr;

/* set up events */
std::vector<SDL_Event> Simulation::frame_events;
SDL_Event Simulation::event;

/* flick "isRunning" switch to ON */
bool Simulation::isRunning = true;
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/ 


void Simulation::init( const char *title, int xpos, int ypos, int width, int height, bool fullscreen )
{
    int flags = 0;
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "subsystems initialized" << std::endl;
        window = SDL_CreateWindow( title, xpos, ypos, width, height, flags );
        if( window )
        {
            std::cout << "Window created" << std::endl;
        }
        
        renderer = SDL_CreateRenderer( window, -1, 0) ;
        if( renderer )
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            std::cout << "Renderer created" << std::endl;
        }
        
    }
    
}


void Simulation::handleEvents()
{
    while (SDL_PollEvent(&event) != 0)
    {   
        frame_events.push_back(event);
    }

    for (int i = 0; i < frame_events.size(); i++)
    {
        switch ( event.type ) {
                case SDL_QUIT:
                    isRunning = false;
                    break;
                    
                default:
                    break;
            }
    }
}


void Simulation::clear_frame_events()
{
    frame_events.clear();
}


void Simulation::update()
{
    
}

void Simulation::clearRenderer() {
    SDL_RenderClear(renderer);
}

void Simulation::render()
{
    SDL_RenderPresent(renderer);
}


void Simulation::clean()
{
    SDL_DestroyWindow( window );
    SDL_DestroyRenderer( renderer );
    SDL_Quit();
    
    std::cout << "Sim cleaned" << std::endl;
}


#endif /* sim_manager_cpp */