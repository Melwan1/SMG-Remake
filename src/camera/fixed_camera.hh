#include "camera/camera.hh"

class FixedCamera : public Camera
{
    public:
        FixedCamera(const cgp::vec3 &eye, const cgp::vec3 &focus, const cgp::vec3 &rotation_axis);

        virtual 
}