#pragma once

#include <filesystem>

#include "cgp/cgp.hpp"

#include <yaml-cpp/yaml.h>

namespace fs = std::filesystem;

class Skybox
{

    public:
        Skybox(const fs::path& texture_path, float distance_from_player);
        Skybox(const YAML::Node &config);

        const fs::path& get_texture_path() const;
        float get_distance_from_player() const;

        void update_mesh_from_camera(const cgp::camera_orbit_euler& camera);
        cgp::mesh get_mesh();
        void initialize_data_on_gpu();
        cgp::mesh_drawable update_drawable();

    private:
    
        int _Nu = 16;
        int _Nv = 16;
        cgp::mesh _mesh;
        cgp::mesh_drawable _drawable;
        fs::path _texture_path;
        float _distance_from_player;

        cgp::numarray<cgp::vec3> _position;
        cgp::numarray<cgp::vec3> _normal;
        cgp::numarray<cgp::uint3> _connectivity;

        std::unique_ptr<cgp::opengl_texture_image_structure> _texture_structure;

};