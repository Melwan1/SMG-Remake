#include "camera/camera.hh"

unsigned Camera::get_id() const
{
    return _id;
}

CameraType Camera::get_camera_type() const
{
    return _camera_type;
}