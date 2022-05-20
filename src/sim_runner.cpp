
#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "constants/static_variables.cpp"
#include "sdl_core/sim_manager.cpp"


#include "ecs/ControlPanel.cpp"

/* components */
#include "ecs/components/transform_component.cpp"
#include "ecs/components/rigid_body_component.cpp"

/* systems */
#include "ecs/systems/transform_system.cpp"
#include "ecs/systems/draw_system.cpp"
#include "ecs/systems/ball_movement_system.cpp"

/* other */
#include <simple_framerate_timer.cpp>
#include "ecs/BallManager.cpp"
#include "ecs/LineManager.cpp"


/*---------- screen dimensions ----------*/
const int screenx = global_const::screen_x;
const int screeny = global_const::screen_y;
/*---------------------------------------*/

ControlPanel control;

/* main function */ 
int main(int argc, const char * argv[]) {
    /*------------------------- SDL setup ----------------------------*/ 
    /* set up time and frames */
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;    
    int frameStart;
    int frameTime;
    
    /* create simulation class instance */ 
    Simulation* simulation = new Simulation;
    simulation->init("PineconeEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenx, screeny, false);
    std::cout << "SDL Simulation successfully initialized." << std::endl;
    /*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
    /*===============================================================================*/
    /*------------------------- SET UP SPECIFICS ------------------------------------*/
    /* set randomness seed */
    // set_srand();

    /* Ordinary Setup items */
    control.Init();

    /* Register Components */
    control.RegisterComponent<RigidBody>();
    control.RegisterComponent<Transform>();


    // /* Register Systems */
    auto transform_system = control.RegisterSystem<pce::TransformSystem>();
    Signature transform_sig;
    transform_sig.set(control.GetComponentType<RigidBody>());
    transform_sig.set(control.GetComponentType<Transform>());
    control.SetSystemSignature<pce::TransformSystem>(transform_sig);
    transform_system->Init();

    auto draw_system = control.RegisterSystem<DrawSystem>();
    Signature draw_sig;
    draw_sig.set(control.GetComponentType<Transform>());
    control.SetSystemSignature<DrawSystem>(draw_sig);

    auto ball_movement_system = control.RegisterSystem<BallMovementSystem>();
    Signature move_sig;
    move_sig.set(control.GetComponentType<RigidBody>());
    control.SetSystemSignature<BallMovementSystem>(move_sig);


    /* Create Entities */
    auto ball_manager = BallManager();
    // auto line_manager = LineManager();

    // for (int i = 0; i < 1; ++i) {
        // ball_manager.MakeBall();
    // }

    // line_manager.Init();
    


    /* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ End Setup */
    /*============================================================================================*/

    simple_framerate_timer simple_timer = simple_framerate_timer();

    /*----------Main Loop---------------------*/

    while (simulation->running()) {
        
        /* SDL events and updating */ 
        int frameStart = SDL_GetTicks();       
        simulation->handleEvents();
        simulation->update();
        simulation->clearRenderer();

        double t = frameStart/1000.0;

        // update timer 
        int time = int(t);
        simple_timer.update_timer(time);

        // print_item("-------------------------------------------");
        /*~~~~~~~~~------------- Do Stuff and Update ----------------*/
        double ticks = (SDL_GetTicks()/1000.0);
        transform_system->UpdateCamera();
        // ball_movement_system->UpdateEntities(ticks);


        /*~~~~~~~~~-------------- Draw and Render --------------------*/
        // line_manager.DrawLines();
        // draw_system->UpdateEntities();
        simulation->render();


        /*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
        /* Clear events list */
        simulation->clear_frame_events();

        /* check how much time has passed during frame */
        frameTime = SDL_GetTicks() - frameStart;

        /* if frame too fast, delay the frame */
        if ( frameDelay > frameTime )
        {
            SDL_Delay( frameDelay - frameTime );
        }
    }

    simulation->clean();

    delete simulation;
    
    return 0;
}

