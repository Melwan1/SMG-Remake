#pragma once

#include "cgp/cgp.hpp"

enum StageType
{
    STAGE_NORMAL,
    STAGE_LUMA,
    STAGE_COMET,
};

class Stage
{
    public:

        Stage(enum StageType stage_type);

        enum StageType get_stage_type() const;

        virtual void start();
        virtual void restart();
        virtual void finish();

        bool is_started() const;
        bool is_finished() const;

    private:

        enum StageType _stage_type;
        bool _started;
        bool _finished;
};