#include "comet_stage.hpp"

CometStage::CometStage(enum CometStageType comet_stage_type)
    : Stage(STAGE_COMET)
    , _comet_stage_type(comet_stage_type)
{}