
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
#include "ecs/components/scaled_rigid_body_component.cpp"
#include "ecs/components/position_component.cpp"
#include "ecs/components/scaled_position_component.cpp"
#include "ecs/components/rotated_location_component.cpp"
#include "ecs/components/sprite_component.cpp"
#include "ecs/components/motion_component.cpp"
// #include "ecs/components/orbit_component.cpp"

/* systems */
#include "ecs/systems/transform_system.cpp"
#include "ecs/systems/draw_system.cpp"
#include "ecs/systems/scale_system.cpp"


/* other */
#include <simple_framerate_timer.cpp>
// #include "ecs/BallManager.cpp"
// #include "ecs/LineManager.cpp"


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
    control.RegisterComponent<ScaledRigidBody>();
    control.RegisterComponent<Position>();
    control.RegisterComponent<ScaledPosition>();
    control.RegisterComponent<Transform>();
    control.RegisterComponent<RotatedLocation>();
    control.RegisterComponent<Motion>();
    control.RegisterComponent<Sprite>();

    // /* Register Systems */
    auto transform_system = control.RegisterSystem<pce::TransformSystem>();
    Signature transform_sig;
    transform_sig.set(control.GetComponentType<ScaledRigidBody>());
    transform_sig.set(control.GetComponentType<ScaledPosition>());
    transform_sig.set(control.GetComponentType<Transform>());
    transform_sig.set(control.GetComponentType<RotatedLocation>());
    control.SetSystemSignature<pce::TransformSystem>(transform_sig);
    transform_system->Init();

    auto draw_system = control.RegisterSystem<DrawSystem>();
    Signature draw_sig;
    draw_sig.set(control.GetComponentType<Transform>());
    draw_sig.set(control.GetComponentType<Sprite>());
    control.SetSystemSignature<DrawSystem>(draw_sig);

    auto scale_system = control.RegisterSystem<pce::ScaleSystem>();
    Signature scale_sig;
    scale_sig.set(control.GetComponentType<RigidBody>());
    scale_sig.set(control.GetComponentType<ScaledRigidBody>());
    scale_sig.set(control.GetComponentType<Position>());
    scale_sig.set(control.GetComponentType<ScaledPosition>());
    control.SetSystemSignature<pce::ScaleSystem>(scale_sig);
    

    // const double sun_scale_minimizer = 4326.9; // sun's radius = 50 pixels
    // const double object_size_booster = 50000000;

    /* Create Entities */
    Entity sun = control.CreateEntity();
    control.AddComponent(sun, RigidBody{
        // .mass_kg=1.989*(10^30),
        .radius_mi=432690.0
    });
    control.AddComponent(sun, ScaledRigidBody{
        .radius=50
    });
    control.AddComponent(sun, Position{
      .center_point=glm::dvec3{0, 0, 0},
    });
    control.AddComponent(sun, ScaledPosition{
      .center_point=glm::dvec3{0, 0, 0},
    });
    control.AddComponent(sun, Sprite{
        .color={255, 206, 0, 255}
    });
    control.AddComponent(sun, RotatedLocation{});
    control.AddComponent(sun, Transform{});

    Entity mercury = control.CreateEntity();
    control.AddComponent(mercury, RigidBody{
        // .mass_kg=3.285*(10^23),
        .radius_mi=1516,
    });
    control.AddComponent(mercury, ScaledRigidBody{
        .radius=10
    });
    control.AddComponent(mercury, Position{
      .center_point=glm::dvec3{43000000.0, 0.0, 0.0}
    });
    control.AddComponent(mercury, ScaledPosition{
      .center_point=glm::dvec3{150.0, 0.0, 0.0}
    });
    control.AddComponent(mercury, Sprite{
        .color={255, 160, 100, 255}
    });
    control.AddComponent(mercury, RotatedLocation{});
    control.AddComponent(mercury, Transform{});




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


        /*~~~~~~~~~-------------- Draw and Render --------------------*/
        scale_system->UpdateEntities();
        transform_system->UpdateEntities();
        draw_system->UpdateEntities();
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

