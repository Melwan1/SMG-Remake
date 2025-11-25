#include "camera/camera.hpp"

#include "deformable/deformable.hpp"

class AttachedToPlayerCamera : public Camera
{
    public:
        AttachedToPlayerCamera(int id, float distance_to_player, const cgp::vec3 &offset);
        AttachedToPlayerCamera(const YAML::Node &config);

        void attach_to_player(shape_deformable_structure &player);
        const shape_deformable_structure* get_attached_player() const;

        virtual const cgp::vec3 get_eye() const override;
        virtual const cgp::vec3 get_focus() const override;
        virtual const cgp::vec3 get_rotation_axis() const override;

    private:
        float _distance_to_player;
        cgp::vec3 _offset;
        shape_deformable_structure *_attached_player = nullptr;
};