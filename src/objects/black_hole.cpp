#include "black_hole.hpp"

BlackHole::BlackHole(float radius, float attraction_radius, cgp::vec3 center)
    : _billboard(Billboard(center, radius))
    , _radius(radius)
    , _attraction_radius(attraction_radius)
    , _center(center)
{}

void BlackHole::update_mesh_from_camera(cgp::camera_orbit_euler camera)
{
    _billboard.update_mesh_from_camera(camera);
}

cgp::mesh_drawable BlackHole::update_drawable()
{
    return _billboard.update_drawable();
}
