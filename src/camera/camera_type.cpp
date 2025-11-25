#include "camera/camera_type.hpp"

#include <map>

CameraType get_camera_type_from_string(const std::string& camera_type_str)
{
    static std::map<std::string, CameraType> map = {
        { "FLOATING", CameraType::FLOATING },
        { "FIXED", CameraType::FIXED },
        { "ATTACHED_TO_PLAYER", CameraType::ATTACHED_TO_PLAYER }
    };

    if (map.find(camera_type_str) == map.end()) {
        return CameraType::ERROR;
    }
    return map[camera_type_str];
}