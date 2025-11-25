#pragma once

#include <filesystem>
#include <memory>

#include "camera/camera.hpp"

namespace fs = std::filesystem;

class CameraLoader
{
    public:

        CameraLoader(const fs::path& camera_path);
        std::unique_ptr<Camera> load();

    private:

        const fs::path _camera_path;
};