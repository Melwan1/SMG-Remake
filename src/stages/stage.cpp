#include "stage.hpp"

Stage::Stage(enum StageType stage_type)
    : _stage_type(stage_type)
{}

void Stage::start()
{
    _started = true;
}

void Stage::restart()
{
    _finished  = false;
    start();
}

void Stage::finish()
{
    if (!is_started())
    {
        return;
    }
    _finished = true;
}

bool Stage::is_started() const
{
    return _started;
}

bool Stage::is_finished() const
{
    return _finished;
}