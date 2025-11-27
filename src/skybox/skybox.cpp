#include "skybox/skybox.hpp"

Skybox::Skybox(const fs::path& texture_path, float distance_from_player)
    : _texture_path("assets/textures" / texture_path)
    , _distance_from_player(distance_from_player)
{
    const float skybox_radius = distance_from_player;
    int Nu = 64;
    int Nv = 64;
    _mesh = cgp::mesh_primitive_sphere(skybox_radius, { 0, 0, 0 }, Nu, Nv);
    
    _texture_structure = std::make_unique<cgp::opengl_texture_image_structure>();

    _texture_structure->load_and_initialize_texture_2d_on_gpu(_texture_path);

    _drawable.clear();
    _drawable.initialize_data_on_gpu(_mesh, cgp::mesh_drawable::default_shader, *_texture_structure);

}

Skybox::Skybox(const YAML::Node &config)
    : Skybox(config["texture_path"].as<std::string>(), config["distance_from_player"].as<float>())
{}

const fs::path& Skybox::get_texture_path() const
{
    return _texture_path;
}

float Skybox::get_distance_from_player() const
{
    return _distance_from_player;
}

void Skybox::update_mesh_from_camera(const cgp::camera_orbit_euler& camera)
{
    _drawable.clear();
    const cgp::vec3 &camera_center = camera.center_of_rotation;
    _mesh = cgp::mesh_primitive_sphere(_distance_from_player, camera_center, _Nu, _Nv);
    initialize_data_on_gpu();
}

cgp::mesh Skybox::get_mesh()
{
    return _mesh;
}

void Skybox::initialize_data_on_gpu()
{
    if (_texture_structure == nullptr)
    {

        _drawable.initialize_data_on_gpu(
            _mesh, cgp::mesh_drawable::default_shader);
    }
    else
    {
        _drawable.initialize_data_on_gpu(
            _mesh, cgp::mesh_drawable::default_shader, *_texture_structure);
    }

    _position = _mesh.position;
    _normal = _mesh.normal;
    _connectivity = _mesh.connectivity;
}

cgp::mesh_drawable Skybox::update_drawable()
{
    _drawable.vbo_position.update(_position);
    cgp::normal_per_vertex(_position, _connectivity, _normal);
    _drawable.vbo_normal.update(_normal);
    return _drawable;
}