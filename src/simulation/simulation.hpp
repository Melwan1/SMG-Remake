#include "../deformable/deformable.hpp"
#include "../objects/planet.hpp"

struct simulation_parameter
{
    // Radius around each vertex considered as a colliding sphere
    float collision_radius = 0.04f;

    // Ratio considered for plastic deformation on the reference shape \in [0,1]
    float plasticity = 0.0f;
    // Ratio considered for elastic deformation
    float elasticity = 0.0f;
    // Velocity reduction at each time step (* dt);
    float friction = 1.0f;
    // Numer of collision handling step for each numerical integration
    int collision_steps = 5;

    // Time step of the numerical time integration
    float time_step = 0.005f;

    float black_hole_timer = 1.0f;
};

void simulation_step(std::vector<shape_deformable_structure> &deformables,
                     const std::vector<Planet> &planets,
                     const std::vector<BlackHole> &black_holes,
                     simulation_parameter const &param,
                     const cgp::camera_orbit_euler &camera);