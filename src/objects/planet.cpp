#include "planet.hpp"

Planet::Planet(float radius, float attraction_radius, cgp::vec3 center,
               int sampling_horizontal, int sampling_vertical)
    : _mesh(cgp::mesh_primitive_sphere(radius, center, sampling_horizontal,
                                       sampling_vertical))
    , _radius(radius)
    , _attraction_radius(attraction_radius)
    , _center(center)
    , _sampling_horizontal(sampling_horizontal)
    , _sampling_vertical(sampling_vertical)
{
    _mesh.centered();
    const cgp::vec3 blue_color({ 0.4, 0.6, 1.0 });
    _mesh.color.fill(blue_color);
    _shape.initialize(_mesh);
    const cgp::vec3 velocity({ 0.0, 0.0, 0.0 });
    const cgp::vec3 angular_velocity({ 0.0, 0.0, 0.0 });
    _shape.set_position_and_velocity(center, velocity, angular_velocity);
}

const cgp::mesh &Planet::get_mesh() const
{
    return _mesh;
}

float Planet::get_radius() const
{
    return _radius;
}

float Planet::get_attraction_radius() const
{
    return _attraction_radius;
}

const cgp::vec3 &Planet::get_center() const
{
    return _center;
}

int Planet::get_sampling_horizontal() const
{
    return _sampling_horizontal;
}

int Planet::get_sampling_vertical() const
{
    return _sampling_vertical;
}

shape_deformable_structure &Planet::get_shape()
{
    return _shape;
}

const shape_deformable_structure &Planet::get_shape() const
{
    return _shape;
}