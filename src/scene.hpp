#pragma once

#include <filesystem>
#include <memory>

#include <yaml-cpp/yaml.h>

#include "environment.hpp"
#include "objects/black_hole.hpp"
#include "objects/planet.hpp"
#include "simulation/simulation.hpp"
#include "camera/camera.hpp"

using cgp::mesh_drawable;

namespace fs = std::filesystem;

enum primitive_type_enum
{
    primitive_cube,
    primitive_cylinder,
    primitive_cone,
    primitive_bunny,
    primitive_spot
};

struct gui_parameters
{
    bool display_frame = true;
    bool display_wireframe = true;
    bool display_collision_sphere = false;
    bool display_walls = true;
    primitive_type_enum primitive_type;
    float throwing_speed = 10.0f;
};

// The structure of the custom scene
struct scene_structure : scene_inputs_generic
{
    // ****************************** //
    // Elements and shapes of the scene
    // ****************************** //
    std::unique_ptr<Camera> _camera_ptr = nullptr;
    camera_controller_orbit_euler camera_control;
    camera_projection_perspective camera_projection;
    window_structure window;

    mesh_drawable global_frame; // The standard global frame
    environment_structure environment; // Standard environment controler
    input_devices
        inputs; // Storage for inputs status (mouse, keyboard, window dimension)
    gui_parameters gui; // Standard GUI element storage

    // ****************************** //
    // Elements and shapes of the scene
    // ****************************** //
    cgp::timer_basic timer;

    simulation_parameter param;
    std::vector<shape_deformable_structure> deformables;
    std::vector<Planet> planets = std::vector<Planet>();
    std::vector<BlackHole> black_holes = std::vector<BlackHole>();
    std::unique_ptr<opengl_texture_image_structure> black_hole_opengl_image;

    void add_new_deformable_shape(vec3 const &center, vec3 const &velocity,
                                  vec3 const &angular_velocity,
                                  vec3 const &color);

    mesh_drawable sphere;
    mesh_drawable wall;
    void throw_new_deformable_shape();

    // ****************************** //
    // Functions
    // ****************************** //

    void initialize_camera(const fs::path &camera_path, const YAML::Node& camera_config);
    void initialize_player(const YAML::Node &player_config);
    void initialize_planets(const YAML::Node &planets_config);
    void initialize_black_holes(const YAML::Node &black_holes_config);

    void initialize(const fs::path& filename); // Standard initialization to be called before the
                       // animation loop
    void
    display_frame(); // The frame display to be called within the animation loop
    void display_gui(); // The display of the GUI, also called within the
                        // animation loop

    void mouse_move_event();
    void mouse_click_event();
    void keyboard_event();
    void idle_frame();
};
