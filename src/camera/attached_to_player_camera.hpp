#include "camera/camera.hpp"

class AttachedToPlayerCamera : public Camera
{
    public:
        AttachedToPlayerCamera(int id, float distance_to_player, const cgp::vec3 &offset);
        AttachedToPlayerCamera(const YAML::Node &config);

        virtual const cgp::vec3 get_eye() const override;
        virtual const cgp::vec3 get_focus() const override;
        virtual const cgp::vec3 get_rotation_axis() const override;

    private:
        float _distance_to_player;
        cgp::vec3 _offset;
};