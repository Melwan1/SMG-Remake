#include "camera/floating_camera.hpp"

FloatingCamera::FloatingCamera(int id, const cgp::vec3 &eye, const cgp::vec3 &focus, const cgp::vec3 &rotation_axis)
    : Camera(id, CameraType::FLOATING)
    , _eye(eye)
    , _focus(focus)
    , _rotation_axis(rotation_axis)
{}

FloatingCamera::FloatingCamera(const YAML::Node &config)
    : Camera(config, CameraType::FLOATING)
{
    const YAML::Node eye_config = config["eye"];
    const YAML::Node focus_config = config["focus"];
    const YAML::Node rotation_axis_config = config["rotation_axis"];

    _eye = {
        eye_config["x"].as<float>(),
        eye_config["y"].as<float>(),
        eye_config["z"].as<float>()
    };

    _focus = {
        focus_config["x"].as<float>(),
        focus_config["y"].as<float>(),
        focus_config["z"].as<float>()
    };

    _rotation_axis = {
        rotation_axis_config["x"].as<float>(),
        rotation_axis_config["y"].as<float>(),
        rotation_axis_config["z"].as<float>()
    };
}

const cgp::vec3 FloatingCamera::get_eye() const
{
    return _eye;
}

const cgp::vec3 FloatingCamera::get_focus() const
{
    return _focus;
}

const cgp::vec3 FloatingCamera::get_rotation_axis() const
{
    return _rotation_axis;   
}