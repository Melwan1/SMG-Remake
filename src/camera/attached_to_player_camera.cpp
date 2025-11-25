#include "camera/attached_to_player_camera.hpp"

#include "cgp/cgp.hpp"

AttachedToPlayerCamera::AttachedToPlayerCamera(int id, float distance_to_player, const cgp::vec3 &offset)
    : Camera(id, CameraType::ATTACHED_TO_PLAYER)
    , _distance_to_player(distance_to_player)
    , _offset(offset)
{}

AttachedToPlayerCamera::AttachedToPlayerCamera(const YAML::Node &config)
    : Camera(config["id"].as<int>(), CameraType::ATTACHED_TO_PLAYER)
{
    _distance_to_player = config["distance_to_player"].as<float>();
    const YAML::Node &offset_config  = config["offset"];

    _offset = {
        offset_config["x"].as<float>(),
        offset_config["y"].as<float>(),
        offset_config["z"].as<float>()
    };
}

void AttachedToPlayerCamera::attach_to_player(shape_deformable_structure& player)
{
    _attached_player = &player;
}

const shape_deformable_structure *AttachedToPlayerCamera::get_attached_player() const
{
    return _attached_player;
}

const cgp::vec3 AttachedToPlayerCamera::get_eye() const
{
    return { 0, 0, 0 };
}

const cgp::vec3 AttachedToPlayerCamera::get_focus() const
{
    return { 0, 0, 0 };
}

const cgp::vec3 AttachedToPlayerCamera::get_rotation_axis() const
{
    return { 0, 1, 0 };
}