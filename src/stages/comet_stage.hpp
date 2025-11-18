#include "stage.hpp"

enum CometStageType
{
    COMET_STAGE_TIMER,
    COMET_STAGE_PURPLE_COINS,
    COMET_STAGE_DAREDEVIL,
};

class CometStage : public Stage
{
    public:

        CometStage(enum CometStageType comet_stage_type);

        enum CometStageType get_comet_stage_type() const;

    private:

        enum CometStageType _comet_stage_type;
};