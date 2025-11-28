#include "simulation.hpp"

#include "../../third_party/eigen/Eigen/Core"
#include "../../third_party/eigen/Eigen/SVD"
#include "deformable/deformable.hpp"
#include "objects/black_hole.hpp"
#include "objects/planet.hpp"

#define PLAYER_CONTINUOUS_DISPLACEMENT { 0.01, 0, 0 }

using namespace cgp;

// Compute the polar decomposition of the matrix M and return the rotation such
// that
//   M = R * S, where R is a rotation matrix and S is a positive semi-definite
//   matrix
mat3 polar_decomposition(mat3 const &M);

void planetary_attraction(std::vector<shape_deformable_structure> &deformables,
                          const std::vector<Planet> &planets,
                          const std::vector<BlackHole> &black_holes,
                          simulation_parameter const &param);

// Compute the collision between the particles and the walls
void collision_with_walls(std::vector<shape_deformable_structure> &deformables);

// Compute the collision between the particles and the planets
void collision_with_planets(
    std::vector<shape_deformable_structure> &deformables,
    const std::vector<Planet> &planets, simulation_parameter const &param);

// Compute the collision between the particles and the black_holes
void collision_with_black_holes(
    std::vector<shape_deformable_structure> &deformables,
    const std::vector<BlackHole> &black_holes,
    simulation_parameter const &param);

// Compute the collision between the particles to each other
void collision_between_particles(
    std::vector<shape_deformable_structure> &deformables,
    simulation_parameter const &param);

// Compute the shape matching on all the deformable shapes
void shape_matching(std::vector<shape_deformable_structure> &deformables,
                    simulation_parameter const &param);

// Perform one simulation step (one numerical integration along the time step
// dt) using PPD + Shape Matching
void simulation_step(std::vector<shape_deformable_structure> &deformables,
                     const std::vector<Planet> &planets,
                     const std::vector<BlackHole> &black_holes,
                     simulation_parameter const &param,
                     const cgp::camera_orbit_euler &camera)
{
    float dt = param.time_step;
    int N_deformable = deformables.size();

    // Calculate the center of mass first for later use
    for (shape_deformable_structure &deformable : deformables)
    {
        deformable.com = average(deformable.position);
    }

    // I. - Apply the external forces to the velocity
    //    - Compute the predicted position from this time integration
    // vec3 const gravity = vec3(0.0f, 0.0f, -9.81f);
    // for (int kd = 0; kd < N_deformable; ++kd)
    // { // For all the deformable shapes
    //     shape_deformable_structure &deformable = deformables[kd];
    //     int N_vertex = deformable.position.size();
    //     for (int k = 0; k < N_vertex;
    //          ++k) // For all the vertices of each deformable shape
    //     {
    //         // Standard integration of external forces
    //         //   drag + gravity
    //         deformable.velocity[k] =
    //             deformable.velocity[k] * (1 - dt * param.friction)
    //             + dt * gravity;
    //         //   predicted position
    //         deformable.position_predict[k] =
    //             deformable.position[k] + dt * deformable.velocity[k];
    //     }
    // }

    // I. bis -> planet attraction instead of gravity
    planetary_attraction(deformables, planets, black_holes, param);

    // II. Constraints using PPD
    //     - Collision with the walls (particles/walls)
    //     - Collision between particles (particles/particles)
    //     - Apply shape matching (per shape)
    // Note: The parameter collision_steps can be modified by the gui interface
    for (int k_collision_steps = 0; k_collision_steps < param.collision_steps;
         ++k_collision_steps)
    {
        // collision_with_walls(deformables);
        collision_between_particles(deformables, param);
        collision_with_planets(deformables, planets, param);
        collision_with_black_holes(deformables, black_holes, param);
        shape_matching(deformables, param);
    }

    // III. Final velocity update
    for (int kd = 0; kd < N_deformable; ++kd)
    {
        shape_deformable_structure &deformable = deformables[kd];

        const cgp::mat4 scaling_transform = mat4::build_identity().
                                            apply_translation(-deformable.com).
                                            apply_scaling(
                                                1 - 2 * dt / param.black_hole_timer)
                                            .apply_translation(deformable.com);

        // Got black holed -> spiral animation
        if (deformable.got_black_holed != nullptr)
        {
            const BlackHole *black_hole = deformable.got_black_holed;
            const vec3 to_black_hole_vec = black_hole->get_center() - deformable
                .com;

            for (int k = 0; k < deformable.size(); ++k)
            {
                deformable.position[k] = (scaling_transform * vec4(
                    deformable.position[k], 1.0)).xyz();

                // Update velocity
                deformable.velocity[k] =
                    cgp::cross(to_black_hole_vec,
                               (camera.position() - deformable.position[k])) /
                    dt;

                // Update the vertex position
                deformable.position[k] += 0.015 *
                    cgp::normalize(deformable.
                        velocity[k]) +
                    to_black_hole_vec * 0.02;
            }

            deformable.dt_timer += dt;
        }
        else
        {
            // Normal update of the velocity
            for (int k = 0; k < deformable.size(); ++k)
            {
                // Update velocity
                deformable.velocity[k] =
                    (deformable.position_predict[k] - deformable.position[k]) /
                    dt;

                // Update the vertex position
                deformable.position[k] = deformable.position_predict[k];
            }
        }
    }

    // FIXME: try to move the player forward on the planet

    for (const Planet &planet : planets)
    {
        if (planet.should_attract_deformable(deformables[0]))
        {
            cgp::vec3 normal = cgp::normalize(
                deformables[0].com - planet.get_center());
            cgp::vec3 movement = PLAYER_CONTINUOUS_DISPLACEMENT;
            float movement_amplitude = std::sqrt(
                cgp::dot(movement, movement));
            cgp::vec3 direction = cgp::cross(normal, movement / movement_amplitude);

            cgp::vec3 displacement = direction * movement_amplitude;

            deformables[0].com += displacement * direction;
            for (int k = 0; k < deformables[0].position.size(); k++)
            {
                deformables[0].position[k] += displacement;
            }
        }
    }
}

// Compute the shape matching on all the deformable shapes
void shape_matching(std::vector<shape_deformable_structure> &deformables,
                    simulation_parameter const &param)
{
    // Arguments:
    //   deformables: stores a vector of all the deformable shape
    //   Each deformable shape structure contains
    //        - the predicted position [deformable].position_predicted
    //        - the center of mass [deformable].com
    //        - the reference position [deformable].position_reference
    //        - the center of mass from the reference position
    //        [deformable].com_reference

    // ********************************************** //
    // TO DO: Implement here the Shape Matching
    // ********************************************** //

    // Algorithm:
    //   - For all deformable shapes
    //     - Update the com (center of mass) from the predicted position
    //     - Compute the best rotation R such that p_predicter - com = R
    //     (p_reference-com_reference)
    //        - Compute the matrix M = \sum r r_ref^T
    //            with r  = p_predicted - com
    //                 r_ref = p_reference - com_reference
    //        - Compute R as the polar decomposition of M
    //     - Set the new predicted position as p_predicted = R
    //     (p_reference-com_reference) + com
    //
    //
    // Help:
    //   - A function "mat3 polar_decomposition(mat3 const& M)" that computes
    //   the polar decomposition using SVD is provided.
    //  - The center of mass of the predicted position can be computed as
    //  "average(deformable.position_predicted)"
    //  - A matrix mat3 can be initialized to zeros with the syntax "mat3 M =
    //  mat3::build_zero();"
    //  - The product a * b^tr, where (a,b) are vec3 is also called a tensor
    //  product. It can be computed using the syntax "mat3 M =
    //  tensor_product(a,b)"
    //
    for (shape_deformable_structure &deformable : deformables)
    {
        if (deformable.got_black_holed != nullptr)
        {
            continue;
        }

        deformable.com = average(deformable.position_predict);
        deformable.com_reference = average(deformable.position);
        mat3 T = mat3::build_zero();
        for (int i = 0; i < deformable.position_predict.size(); i++)
        {
            T += tensor_product(deformable.position_predict[i] - deformable.com,
                                deformable.position[i]
                                - deformable.com_reference);
        }
        mat3 R = polar_decomposition(T);
        for (int i = 0; i < deformable.position_predict.size(); i++)
        {
            auto new_pred =
                R * (deformable.position[i] - deformable.com_reference)
                + deformable.com;
            deformable.position_predict[i] =
                param.elasticity * deformable.position_predict[i]
                + (1 - param.elasticity) * new_pred;
        }
    }
}

void collision_between_particles(
    std::vector<shape_deformable_structure> &deformables,
    simulation_parameter const &param)
{
    float r = param.collision_radius; // radius of colliding sphere

    // Prepare acceleration structure using axis-aligned bounding boxes.
    std::vector<bounding_box> bbox;
    int N_deformable = deformables.size();
    for (int kd = 0; kd < N_deformable; ++kd)
    {
        bounding_box b;
        b.initialize(deformables[kd].position_predict);
        b.extends(r);
        bbox.push_back(b);
    }
    // Can test if two bounding box (bbox1,bbox2) collide using
    //   bool is_in_collision = bounding_box::collide(bbox1, bbox2);
    // These bounding box are an optional possibility to accelerate the
    // computation to avoid checking pairwise collisions between particles of
    // shapes that are far away.

    // ********************************************** //
    // TO DO: Implement here the collision between particles from different
    // deformable shapes
    // ********************************************** //

    // Algorithm:
    //  - For all the deformable shapes (d_i,d_j)
    //    - For all the vertices(/particles) of the shapes (p_i,p_j)
    //      - If ||p_i-p_j|| < 2 r // collision state
    //           Then modify (p_i,p_j) to remove the collision state
    for (int i = 0; i < deformables.size(); i++)
    {
        for (int j = 0; j < i; j++)
        {
            auto &left_deformable = deformables[i];
            auto &right_deformable = deformables[j];
            if (bounding_box::collide(bbox[i], bbox[j]))
            {
                // objects MAY collide
                for (auto &p_left : left_deformable.position_predict)
                {
                    for (auto &p_right : right_deformable.position_predict)
                    {
                        auto n = norm(p_left - p_right);
                        if (n < 2 * r)
                        {
                            vec3 left_to_right = (p_right - p_left) / n;
                            float d = 2 * r - n;
                            p_left -= left_to_right * d / 2;
                            p_right += left_to_right * d / 2;
                        }
                    }
                }
            }
        }
    }
}

void collision_with_planets(
    std::vector<shape_deformable_structure> &deformables,
    const std::vector<Planet> &planets, simulation_parameter const &param)
{
    const float r = param.collision_radius; // radius of colliding sphere

    // Prepare acceleration structure using axis-aligned bounding boxes.
    std::vector<bounding_box> bbox;
    const int N_deformable = deformables.size();
    for (int kd = 0; kd < N_deformable; ++kd)
    {
        bounding_box b;
        b.initialize(deformables[kd].position_predict);
        b.extends(r);
        bbox.push_back(b);
    }

    std::vector<bounding_box> planet_bbox;
    const int N_planet = planets.size();
    for (int kp = 0; kp < N_planet; ++kp)
    {
        bounding_box b;

        b.initialize(planets[kp].get_mesh().position);
        // offset the bounding box with the position of the planet
        b.p_min += planets[kp].get_center();
        b.p_max += planets[kp].get_center();
        b.extends(2 * planets[kp].get_radius());
        planet_bbox.push_back(b);
    }

    for (int i = 0; i < N_deformable; i++)
    {
        for (int j = 0; j < N_planet; j++)
        {
            auto &deformable = deformables[i];
            auto &planet = planets[j];
            const auto planet_r = planet.get_radius();
            const auto planet_center = planet.get_center();

            if (bounding_box::collide(bbox[i], planet_bbox[j]))
            {
                // objects MAY collide
                for (auto &deformable_position : deformable.position_predict)
                {
                    vec3 to_planet = planet_center - deformable_position;
                    auto n = norm(to_planet);
                    if (n < r + planet_r)
                    {
                        deformable_position -=
                            normalize(to_planet) * ((r + planet_r) - n);
                    }
                }
            }
        }
    }
}


void collision_with_black_holes(
    std::vector<shape_deformable_structure> &deformables,
    const std::vector<BlackHole> &black_holes,
    simulation_parameter const &param)
{
    const float r = param.collision_radius; // radius of colliding sphere

    // Prepare acceleration structure using axis-aligned bounding boxes.
    std::vector<bounding_box> bbox;
    const int N_deformable = deformables.size();
    for (int kd = 0; kd < N_deformable; ++kd)
    {
        bounding_box b;
        b.initialize(deformables[kd].position_predict);
        b.extends(r);
        bbox.push_back(b);
    }

    std::vector<bounding_box> black_hole_bbox;
    const int N_black_hole = black_holes.size();
    for (int kp = 0; kp < N_black_hole; ++kp)
    {
        bounding_box b;

        b.initialize(numarray<vec3>({ black_holes[kp].get_center() }));
        // offset the bounding box with the position of the black_hole
        b.extends(2 * black_holes[kp].get_radius());
        black_hole_bbox.push_back(b);
    }

    for (int i = 0; i < N_deformable; i++)
    {
        if (deformables[i].got_black_holed != nullptr)
        {
            continue;
        }

        for (int j = 0; j < N_black_hole; j++)
        {
            auto &deformable = deformables[i];
            auto &black_hole = black_holes[j];
            const auto black_hole_r = black_hole.get_radius();
            const auto black_hole_center = black_hole.get_center();

            if (bounding_box::collide(bbox[i], black_hole_bbox[j]))
            {
                // objects MAY collide
                for (auto &deformable_position : deformable.position_predict)
                {
                    vec3 to_black_hole = black_hole_center -
                        deformable_position;
                    auto n = norm(to_black_hole);
                    if (n < r + black_hole_r)
                    {
                        deformable.got_black_holed = &black_hole;
                    }
                }
            }
        }
    }
}

// Compute the collision between the particles and the walls
// Note: This function is already pre-coded
void collision_with_walls(std::vector<shape_deformable_structure> &deformables)
{
    int N_deformable = deformables.size();
    for (int kd = 0; kd < N_deformable; ++kd)
    {
        shape_deformable_structure &deformable = deformables[kd];
        int N_vertex = deformable.size();
        for (int k = 0; k < N_vertex; ++k)
        {
            vec3 &p = deformable.position_predict[k];

            // Standard collision with the walls in x and y.
            //  Modify these values for different parameters
            if (p.x < -1)
                p.x = -1;
            if (p.x > 5)
                p.x = 5;
            if (p.y < -1)
                p.y = -1;
            if (p.y > 5)
                p.y = 5;

            // Collision with the ground
            if (p.z < 0)
            {
                p.z = 0;
                // model friction with the ground
                p.x = deformable.position[k].x;
                p.y = deformable.position[k].y;
            }
        }
    }
}

// Compute the attraction of the planet
void planetary_attraction(std::vector<shape_deformable_structure> &deformables,
                          const std::vector<Planet> &planets,
                          const std::vector<BlackHole> &black_holes,
                          simulation_parameter const &param)
{
    constexpr float G = 6.67 * 1e-11;
    // const vec3 gravity = vec3(0.0f, 0.0f, -9.81f);

    const int N_deformable = deformables.size();
    const float dt = param.time_step;

    for (int kd = 0; kd < N_deformable; ++kd)
    {
        if (deformables[kd].got_black_holed != nullptr)
        {
            continue;
        }

        // For all the deformable shapes
        shape_deformable_structure &deformable = deformables[kd];
        const int N_vertex = deformable.position.size();

        auto combined_gravity = vec3(0.0, 0.0, 0.0);
        for (const auto &planet : planets)
        {
            vec3 planet_vector = planet.get_center() - deformable.com;
            const float n = norm(planet_vector);
            const auto attraction_radius = planet.get_attraction_radius();

            if (n <= attraction_radius)
            {
                constexpr float random_mass_factor = 25;
                // In reality, it's : G * m1 * m2 / (n * n)
                combined_gravity =
                    random_mass_factor * normalize(planet_vector) / (n * n);
                break;
            }
        }

        for (const auto &black_hole : black_holes)
        {
            vec3 black_hole_vector = black_hole.get_center() - deformable.com;
            const float n = norm(black_hole_vector);
            const auto attraction_radius = black_hole.get_attraction_radius();

            if (n <= attraction_radius)
            {
                constexpr float random_mass_factor = 40;
                // In reality, it's : G * m1 * m2 / (n * n)
                combined_gravity =
                    random_mass_factor * normalize(black_hole_vector);
                break;
            }
        }

        for (int k = 0; k < N_vertex; ++k)
        {
            // For all the vertices of each deformable shape

            // Standard integration of external forces
            //   drag + gravity
            deformable.velocity[k] =
                deformable.velocity[k] * (1 - dt * param.friction)
                + dt * combined_gravity;
            //   predicted position
            deformable.position_predict[k] =
                deformable.position[k] + dt * deformable.velocity[k];
        }
    }
}

// Compute the polar decomposition of the matrix M and return the rotation such
// that
//   M = R * S, where R is a rotation matrix and S is a positive semi-definite
//   matrix
mat3 polar_decomposition(mat3 const &M)
{
    // The function uses Eigen to compute the SVD of the matrix M
    //  Give: SVD(M) = U Sigma V^tr
    //  We have: R = U V^tr, and S = V Sigma V^tr
    Eigen::Matrix3f A;
    A << M(0, 0), M(0, 1), M(0, 2), M(1, 0), M(1, 1), M(1, 2), M(2, 0), M(2, 1),
        M(2, 2);
    Eigen::JacobiSVD<Eigen::Matrix3f> svd(
        A, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::Matrix3f const R = svd.matrixU() * (svd.matrixV().transpose());

    return { R(0, 0), R(0, 1), R(0, 2), R(1, 0), R(1, 1),
             R(1, 2), R(2, 0), R(2, 1), R(2, 2) };
}