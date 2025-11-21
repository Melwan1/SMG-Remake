#pragma once

#include "deformable/deformable.hpp"

#define PLANET_DEFAULT_SAMPLING_HORIZONTAL 32
#define PLANET_DEFAULT_SAMPLING_VERTICAL 32

class Planet
{
public:
    Planet(float radius, float attraction_radius, cgp::vec3 center,
           int sampling_horizontal = PLANET_DEFAULT_SAMPLING_HORIZONTAL,
           int sampling_vertical = PLANET_DEFAULT_SAMPLING_VERTICAL);

    const cgp::mesh &get_mesh() const;
    float get_radius() const;
    float get_attraction_radius() const;
    const cgp::vec3 &get_center() const;
    int get_sampling_horizontal() const;
    int get_sampling_vertical() const;

    bool should_attract_deformable(const shape_deformable_structure &deformable) const;
    const shape_deformable_structure &get_shape() const;
    shape_deformable_structure &get_shape();

private:
    cgp::mesh _mesh;
    float _radius;
    float _attraction_radius;
    cgp::vec3 _center;
    int _sampling_horizontal;
    int _sampling_vertical;

    shape_deformable_structure _shape;
};