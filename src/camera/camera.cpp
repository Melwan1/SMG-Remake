#include "camera/camera.hpp"

Camera::Camera(int id, CameraType camera_type)
    : _id(id)
    , _camera_type(camera_type)
{}

Camera::Camera(const YAML::Node &config, CameraType camera_type)
{
    _id = config["id"].as<int>();
    _camera_type = camera_type;
}

unsigned Camera::get_id() const
{
    return _id;
}

CameraType Camera::get_camera_type() const
{
    return _camera_type;
}