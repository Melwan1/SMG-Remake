#include "luma_stage.hpp"

LumaStage::LumaStage(int requirements)
    : Stage(STAGE_LUMA)
    , _requirements(requirements)
{}

int LumaStage::get_requirements() const
{
    return _requirements;
}