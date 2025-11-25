#pragma once

#include "cgp/cgp.hpp"

class Billboard
{
public:
    Billboard(cgp::vec3 origin, float size);
    Billboard(cgp::vec3 origin, float size, cgp::camera_orbit_euler camera);

    void update_mesh_from_camera(cgp::camera_orbit_euler camera);
    cgp::mesh get_mesh();

    void initialize_data_on_gpu();
    cgp::mesh_drawable update_drawable();


private:
    cgp::vec3 _origin;
    float _size;

    cgp::mesh _mesh;
    cgp::mesh_drawable _drawable;

    cgp::numarray<cgp::vec3> position;
    cgp::numarray<cgp::vec3> normal;
    cgp::numarray<cgp::uint3> connectivity;
};