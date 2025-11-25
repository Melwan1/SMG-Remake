#pragma once

#include "camera/camera.hpp"


class FloatingCamera : public Camera
{
    public:
        FloatingCamera(int id, const cgp::vec3 &eye, const cgp::vec3 &focus, const cgp::vec3 &rotation_axis);
        FloatingCamera(const YAML::Node &config);

        virtual const cgp::vec3 get_eye() const override;
        virtual const cgp::vec3 get_focus() const override;
        virtual const cgp::vec3 get_rotation_axis() const override;
    
    private:

        cgp::vec3 _eye;
        cgp::vec3 _focus;
        cgp::vec3 _rotation_axis;
};