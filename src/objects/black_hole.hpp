#pragma once

#include "billboard.hpp"
#include "cgp/cgp.hpp"

class BlackHole
{
public:
    BlackHole(float radius, float attraction_radius, cgp::vec3 center);
    void update_mesh_from_camera(cgp::camera_orbit_euler camera);
    cgp::mesh_drawable update_drawable();

private:
    Billboard _billboard;
    float _radius;
    float _attraction_radius;
    cgp::vec3 _center;
};
