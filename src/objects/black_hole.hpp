#pragma once

#include "billboard.hpp"
#include "cgp/cgp.hpp"

class BlackHole
{
public:
    static cgp::opengl_texture_image_structure *black_hole_global_texture;

    BlackHole(float radius, float attraction_radius, cgp::vec3 center);
    void update_mesh_from_camera(cgp::camera_orbit_euler camera);
    cgp::mesh_drawable update_drawable();

    float get_radius() const;
    float get_attraction_radius() const;
    const cgp::vec3 &get_center() const;

private:
    Billboard _billboard;
    float _radius;
    float _attraction_radius;
    cgp::vec3 _center;
};

static std::string black_hole_global_texture_path =
    "assets/textures/SMG_Asset_Sprite_Black_Hole.png";