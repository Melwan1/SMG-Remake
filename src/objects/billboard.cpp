#include "billboard.hpp"

Billboard::Billboard(cgp::vec3 origin, float size)
    : _origin(origin)
    , _size(size)
{}

Billboard::Billboard(cgp::vec3 origin, float size,
                     cgp::camera_orbit_euler camera)
    : Billboard(origin, size)
{}

void Billboard::update_mesh_from_camera(cgp::camera_orbit_euler camera)
{
    _drawable.clear();

    cgp::vec3 right = camera.right();
    cgp::vec3 up = camera.up();

    const float d = _size / 2;
    cgp::vec3 p00 = _origin + d * (-right - up);
    cgp::vec3 p10 = _origin + d * (right - up);
    cgp::vec3 p11 = _origin + d * (right + up);
    cgp::vec3 p01 = _origin + d * (-right + up);

    _mesh = cgp::mesh_primitive_quadrangle(p00, p10, p11, p01);
    initialize_data_on_gpu();
}

cgp::mesh Billboard::get_mesh()
{
    return _mesh;
}

void Billboard::initialize_data_on_gpu()
{
    _drawable.initialize_data_on_gpu(_mesh);

    position = _mesh.position;
    normal = _mesh.normal;
    connectivity = _mesh.connectivity;
}

cgp::mesh_drawable Billboard::update_drawable()
{
    _drawable.vbo_position.update(position);
    normal_per_vertex(position, connectivity, normal);
    _drawable.vbo_normal.update(normal);
    return _drawable;
}
