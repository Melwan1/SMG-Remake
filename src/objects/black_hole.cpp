#include "black_hole.hpp"

cgp::opengl_texture_image_structure *BlackHole::black_hole_global_texture =
    nullptr;

BlackHole::BlackHole(float radius, float attraction_radius, cgp::vec3 center)
    : _billboard(Billboard(center, radius))
      , _radius(radius)
      , _attraction_radius(attraction_radius)
      , _center(center)
{
    _billboard.set_texture(black_hole_global_texture);
}

void BlackHole::update_mesh_from_camera(cgp::camera_orbit_euler camera)
{
    _billboard.update_mesh_from_camera(camera);
}

cgp::mesh_drawable BlackHole::update_drawable()
{
    return _billboard.update_drawable();
}

float BlackHole::get_radius() const
{
    return _radius;
}

float BlackHole::get_attraction_radius() const
{
    return _attraction_radius;
}

const cgp::vec3 &BlackHole::get_center() const
{
    return _center;
}