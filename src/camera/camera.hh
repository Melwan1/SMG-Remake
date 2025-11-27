#pragma once

#include "cgp/cgp.hh"

#include "camera/camera_type.hh"

#include <filesystem>
#include <memory>

namespace fs = std::filesystem;

template <typename CameraObjectType>
class Camera
{

    public:
        Camera() = default;
        virtual ~Camera() = default;

        template <typename CameraObjectT>
        static std::unique_ptr<CameraObjectT> load_from_file(const fs::path& input_file);

        unsigned get_id() const;
        CameraType get_camera_type() const;

        virtual cgp::vec3 get_eye() const = 0;
        virtual cgp::vec3 get_focus() const = 0;
        virtual cgp::vec3 get_rotation_axis() const = 0;

    private:

        unsigned _id;
        CameraType _camera_type;

};