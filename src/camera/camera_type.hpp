#pragma once

#include <string>

enum class CameraType
{
    FLOATING = 0,
    FIXED,
    ATTACHED_TO_PLAYER,
    ERROR = -1,
};

CameraType get_camera_type_from_string(const std::string& camera_type_str);