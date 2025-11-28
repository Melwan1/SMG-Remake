#pragma once

#include "../../third_party/eigen/Eigen/Core"
#include "../../third_party/eigen/Eigen/SVD"
#include "camera/camera.hpp"
#include "deformable/deformable.hpp"
#include "objects/black_hole.hpp"
#include "objects/planet.hpp"

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


// Compute the polar decomposition of the matrix M and return the rotation such
// that
//   M = R * S, where R is a rotation matrix and S is a positive semi-definite
//   matrix
cgp::mat3 polar_decomposition(cgp::mat3 const &M);

void planetary_attraction(std::vector<std::shared_ptr<shape_deformable_structure>> &deformables,
                          const std::vector<Planet> &planets,
                          const std::vector<BlackHole> &black_holes,
                          simulation_parameter const &param);

// Compute the collision between the particles and the walls
void collision_with_walls(std::vector<std::shared_ptr<shape_deformable_structure>> &deformables);

// Compute the collision between the particles and the planets
void collision_with_planets(
    std::vector<std::shared_ptr<shape_deformable_structure>> &deformables,
    const std::vector<Planet> &planets, simulation_parameter const &param);

// Compute the collision between the particles and the black_holes
void collision_with_black_holes(
    std::vector<std::shared_ptr<shape_deformable_structure>> &deformables,
    const std::vector<BlackHole> &black_holes,
    simulation_parameter const &param);

// Compute the collision between the particles to each other
void collision_between_particles(
    std::vector<std::shared_ptr<shape_deformable_structure>> &deformables,
    simulation_parameter const &param);

// Compute the shape matching on all the deformable shapes
void shape_matching(std::vector<std::shared_ptr<shape_deformable_structure>> &deformables,
                    simulation_parameter const &param);

// Compute player movement
void player_movement(shape_deformable_structure &player,
                     const std::vector<Planet>& planets,
                     Camera *camera, cgp::input_devices &inputs);