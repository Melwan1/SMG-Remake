#pragma once

#include "cgp/cgp.hpp"

#include "camera/camera_type.hpp"

#include <filesystem>
#include <memory>

#include <yaml-cpp/yaml.h>

namespace fs = std::filesystem;

class Camera
{

    public:
        Camera(int id, CameraType camera_type);
        Camera(const YAML::Node &config, CameraType camera_type);
        virtual ~Camera() = default;

        unsigned get_id() const;
        CameraType get_camera_type() const;

        virtual const cgp::vec3 get_eye() const = 0;
        virtual const cgp::vec3 get_focus() const = 0;
        virtual const cgp::vec3 get_rotation_axis() const = 0;

    private:

        unsigned _id;
        CameraType _camera_type;

};