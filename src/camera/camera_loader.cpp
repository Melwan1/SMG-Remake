#include "camera/camera_loader.hpp"

#include "camera/fixed_camera.hpp"
#include "camera/floating_camera.hpp"
#include "camera/attached_to_player_camera.hpp"

#include <yaml-cpp/yaml.h>

#include "cgp/cgp.hpp"

CameraLoader::CameraLoader(const fs::path &camera_path)
    : _camera_path("config/camera/" / camera_path)
{}

std::unique_ptr<Camera> CameraLoader::load()
{
    const YAML::Node camera_config = YAML::LoadFile(_camera_path);
    const std::string camera_type_str = camera_config["type"].as<std::string>();
    CameraType camera_type = get_camera_type_from_string(camera_type_str);
    switch (camera_type) {
        case CameraType::FIXED:
            return std::move(std::make_unique<FixedCamera>(camera_config));
        case CameraType::FLOATING:
            return std::move(std::make_unique<FloatingCamera>(camera_config));
        case CameraType::ATTACHED_TO_PLAYER:
            return std::move(std::make_unique<AttachedToPlayerCamera>(camera_config));
        case CameraType::ERROR:
            return nullptr;
    }
}