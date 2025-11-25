#include "scene.hpp"

void scene_structure::initialize()
{
    YAML::Node scene_config = YAML::LoadFile("config/scenes/scene_02.yaml");
    YAML::Node planet_config = scene_config["planets"];
    YAML::Node black_hole_config = scene_config["black_holes"];
    initialize_camera(scene_config["camera"]);
    global_frame.initialize_data_on_gpu(mesh_primitive_frame());

    // A sphere used to display the collision model
    sphere.initialize_data_on_gpu(
        mesh_primitive_sphere(1.0f, { 0, 0, 0 }, 10, 5));

    initialize_player(scene_config["player"]);
    initialize_planets(planet_config);
    initialize_black_holes(black_hole_config);

}

void scene_structure::initialize_camera(const YAML::Node& camera_config)
{
    camera_control.initialize(inputs,
                              window); // Give access to the inputs and window
                                       // global state to the camera controler
    camera_control.set_rotation_axis_z();
    YAML::Node eye_config = camera_config["eye"];
    const cgp::vec3 eye = { eye_config["x"].as<float>(), eye_config["y"].as<float>(), eye_config["z"].as<float>() };
    YAML::Node focus_config = camera_config["focus"];
    const cgp::vec3 focus = { focus_config["x"].as<float>(), focus_config["y"].as<float>(), focus_config["z"].as<float>() };
    camera_control.look_at(eye, focus);
}

void scene_structure::initialize_player(const YAML::Node &player_config)
{
    const YAML::Node player_position_config = player_config["position"];
    const cgp::vec3 player_position = { player_position_config["x"].as<float>(), player_position_config["y"].as<float>(), player_position_config["z"].as<float>() };
    const YAML::Node player_size_config = player_config["size"];
    const cgp::vec3 player_size = { player_size_config["x"].as<float>(), player_size_config["y"].as<float>(), player_size_config["z"].as<float>() };
    shape_deformable_structure player;
    player.initialize(mesh_primitive_ellipsoid(player_size));
    player.set_position_and_velocity(player_position);
    deformables.push_back(player);
}

void scene_structure::initialize_planets(const YAML::Node &planets_config)
{
    for (auto planet_id_iterator = planets_config.begin(); planet_id_iterator != planets_config.end(); planet_id_iterator++)
    {
        int planet_id = planet_id_iterator->as<int>();
        std::ostringstream filename;
        filename << "config/planets/planet_" << std::setw(2) << std::setfill('0') << planet_id << ".yaml";
        YAML::Node planet_config = YAML::LoadFile(filename.str());

        const YAML::Node planet_position_config = planet_config["position"];
        const cgp::vec3 planet_position = { planet_position_config["x"].as<float>(), planet_position_config["y"].as<float>(), planet_position_config["z"].as<float>() };
        const float planet_radius = planet_config["radius"].as<float>();
        const float planet_attraction_radius = planet_config["attraction_radius"].as<float>();

        planets.emplace_back(planet_radius, planet_attraction_radius, planet_position);
    }
}

void scene_structure::initialize_black_holes(const YAML::Node &black_holes_config)
{
    for (auto black_hole_id_iterator = black_holes_config.begin(); black_hole_id_iterator != black_holes_config.end(); black_hole_id_iterator++)
    {
        int black_hole_id = black_hole_id_iterator->as<int>();
        std::ostringstream filename;
        filename << "config/black_holes/black_hole_" << std::setw(2) << std::setfill('0') << black_hole_id << ".yaml";
        YAML::Node black_hole_config = YAML::LoadFile(filename.str());

        const YAML::Node black_hole_position_config = black_hole_config["position"];
        const cgp::vec3 black_hole_position = { black_hole_position_config["x"].as<float>(), black_hole_position_config["y"].as<float>(), black_hole_position_config["z"].as<float>() };
        const float black_hole_radius = black_hole_config["radius"].as<float>();
        const float black_hole_attraction_radius = black_hole_config["attraction_radius"].as<float>();

        black_holes.emplace_back(black_hole_radius, black_hole_attraction_radius, black_hole_position);
    }
}

void scene_structure::display_frame()
{
    // Set the light to the current position of the camera
    environment.light = camera_control.camera_model.position();

    if (gui.display_frame)
        draw(global_frame, environment);

    timer.update();

    // Compute the simulation
    if (param.time_step > 1e-6f)
    {
        simulation_step(deformables, planets, param);
    }

    for (int planet_index = 0; planet_index < planets.size(); planet_index++)
    {
        if (planets[planet_index].should_attract_deformable(deformables[0]))
        {
            cgp::vec3 normal = normalize(deformables[0].com - planets[planet_index].get_center());
            //camera_control.look_at(deformables[0].com + normal * 1.6 * planets[planet_index].get_radius(), deformables[0].com);
        }
    }

    // Display all the deformable shapes
    for (int k = 0; k < deformables.size(); ++k)
    {
        deformables[k].update_drawable();
        draw(deformables[k].drawable);
        if (gui.display_wireframe)
        {
            draw_wireframe(deformables[k].drawable);
        }
    }

    // display the black holes

    for (int black_hole_index = 0; black_hole_index < black_holes.size(); black_hole_index++)
    {
        black_holes[black_hole_index].update_mesh_from_camera(camera_control.camera_model);
        cgp::mesh_drawable drawable = black_holes[black_hole_index].update_drawable();
        draw(drawable);
        if (gui.display_wireframe)
        {
            draw_wireframe(drawable);
        }
    }

    // display the planets

    for (int planet_index = 0; planet_index < planets.size(); planet_index++)
    {
        shape_deformable_structure &shape = planets[planet_index].get_shape();
        shape.update_drawable();
        draw(shape.drawable);
        if (gui.display_wireframe)
        {
            draw_wireframe(shape.drawable);
        }
    }

    // Display the walls and the ground
    if (gui.display_walls)
        draw(wall);

    // Display the vertices with their colliding spheres (slower the display for
    // many vertices)
    if (gui.display_collision_sphere)
    {
        for (int k = 0; k < deformables.size(); ++k)
        {
            sphere.model.scaling = param.collision_radius;
            for (int kv = 0; kv < deformables[k].position.size(); ++kv)
            {
                sphere.model.translation = deformables[k].position[kv];
                draw(sphere, environment);
            }
        }
    }
}

void scene_structure::display_gui()
{
    ImGui::Checkbox("Frame", &gui.display_frame);

    ImGui::Checkbox("Wireframe", &gui.display_wireframe);
    ImGui::Checkbox("Walls", &gui.display_walls);

    ImGui::Spacing();
    ImGui::Checkbox("Display collision spheres", &gui.display_collision_sphere);
    ImGui::SliderFloat("Collision radius", &param.collision_radius, 0.001f,
                       0.1f);

    ImGui::Spacing();
    ImGui::Text("Primitive type:"); // Select surface to be created
    int *ptr_primitive_type = reinterpret_cast<int *>(
        &gui.primitive_type); // trick - use pointer to select enum
    ImGui::RadioButton("Cube", ptr_primitive_type, primitive_cube);
    ImGui::SameLine();
    ImGui::RadioButton("Cylinder", ptr_primitive_type, primitive_cylinder);
    ImGui::SameLine();
    ImGui::RadioButton("Cone", ptr_primitive_type, primitive_cone);
    ImGui::SameLine();
    ImGui::RadioButton("Bunny", ptr_primitive_type, primitive_bunny);
    ImGui::SameLine();
    ImGui::RadioButton("Spot", ptr_primitive_type, primitive_spot);

    ImGui::Spacing();
    ImGui::SliderFloat("Time step", &param.time_step, 0, 0.01f, "%.5f", 2.0f);
    ImGui::SliderInt("Collision steps", &param.collision_steps, 1, 10);
    ImGui::SliderFloat("Friction with air", &param.friction, 0.001f, 0.1f,
                       "%.4f", 2);
    ImGui::SliderFloat("Elasticity", &param.elasticity, 0, 1);
    ImGui::SliderFloat("Plasticity", &param.plasticity, 0, 1);

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::SliderFloat("Speed new shape", &gui.throwing_speed, 0.0f, 40.0f);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 100, 100, 255));
    if (ImGui::Button("Add deformable shape (SPACE)"))
    {
        throw_new_deformable_shape();
    }
    ImGui::PopStyleColor();
}

// Generate a new deformable shape appearing in front of the camera with an
// initial velocity
void scene_structure::throw_new_deformable_shape()
{
    // Set its position in from of the camera
    vec3 p0 = camera_control.camera_model.position();
    // Give an initial linear velocity going in front of the camera. Its speed s
    // is set via the gui.throwing speed parameter.
    float s = gui.throwing_speed;
    vec3 v0 = s * camera_control.camera_model.front();
    // Set a random initial angular velocity
    vec3 angular_velocity =
        vec3(rand_uniform(-s, s), rand_uniform(-s, s), rand_uniform(-s, s));

    // A lookup table for the color
    static std::vector<vec3> color_lut = { { 1, 0.5, 0.5 }, { 0.5, 1, 0.5 },
                                           { 0.5, 0.5, 1 }, { 1, 1, 0.5 },
                                           { 1, 0.5, 1 },   { 0.5, 1, 1 } };
    vec3 color = color_lut[int(rand_uniform(0, color_lut.size()))];

    // Create the new deformable shape
    add_new_deformable_shape(p0, v0, angular_velocity, color);
}

void scene_structure::add_new_deformable_shape(vec3 const &center,
                                               vec3 const &velocity,
                                               vec3 const &angular_velocity,
                                               vec3 const &color)
{
    // Initialize default primitive mesh
    mesh m;
    switch (gui.primitive_type)
    {
    case primitive_cube: {
        float L = 0.1;
        int N_sample = 5;
        m = mesh_primitive_cubic_grid(
            { -L, -L, -L }, { L, -L, -L }, { L, L, -L }, { -L, L, -L },
            { -L, -L, L }, { L, -L, L }, { L, L, L }, { -L, L, L }, N_sample,
            N_sample, N_sample);
        m.color.fill(color);
        break;
    }
    case primitive_cylinder: {
        int N_sample = 10;
        float L = 0.1f;
        float r = 0.1f;
        m = mesh_primitive_cylinder(r, { -L, 0, 0 }, { L, 0, 0 }, N_sample,
                                    2 * N_sample);
        m.color.fill(color);
        break;
    }
    case primitive_cone: {
        float L = 0.1;
        int N_sample = 8;
        m = mesh_primitive_cone(L, 2 * L, { 0, 0, 0 }, { 0, 0, 1 }, false,
                                2 * N_sample, 8);
        m.color.fill(color);
        break;
    }
    case primitive_bunny: {
        m = mesh_load_file_obj(project::path + "assets/bunny.obj");
        m.scale(1.5f);
        m.flip_connectivity();
        m.color.fill(color);
        break;
    }
    case primitive_spot: {
        m = mesh_load_file_obj(project::path + "assets/spot.obj");
        m.scale(0.25f);
        break;
    }
    }

    // center the mesh in case it is not already
    m.centered();

    // Create a deformable structure from the mesh
    shape_deformable_structure deformable;
    deformable.initialize(m);
    deformable.set_position_and_velocity(center, velocity, angular_velocity);

    // Special case for spot: set the texture
    if (gui.primitive_type == primitive_spot)
    {
        deformable.drawable.texture.load_and_initialize_texture_2d_on_gpu(
            project::path + "assets/spot_texture.png");
    }

    // Add the new deformable structure
    deformables.push_back(deformable);
}

void scene_structure::mouse_move_event()
{
    if (!inputs.keyboard.shift)
        camera_control.action_mouse_move(environment.camera_view);
}
void scene_structure::mouse_click_event()
{
    camera_control.action_mouse_click(environment.camera_view);
}
void scene_structure::keyboard_event()
{
    camera_control.action_keyboard(environment.camera_view);

    if (inputs.keyboard.last_action.is_pressed(GLFW_KEY_SPACE))
    {
        throw_new_deformable_shape();
    }
}
void scene_structure::idle_frame()
{
    camera_control.idle_frame(environment.camera_view);
}
