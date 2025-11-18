#include "timer_comet_stage.hpp"

#include <sstream>

TimerCometStage::TimerCometStage(int timer_seconds)
    : CometStage(COMET_STAGE_TIMER)
    , _timer_seconds(timer_seconds)
{}

int TimerCometStage::get_time_left() const
{
    if (!is_started())
    {
        return -1;
    }
    std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
    if (is_timer_up())
    {
        return 0;
    }
    int milliseconds_per_second = 1000;

    // round up the milliseconds to the closest second
    return (std::chrono::duration_cast<std::chrono::milliseconds>(_timer_end - current_time).count() + milliseconds_per_second - 1) / milliseconds_per_second;
}

bool TimerCometStage::is_timer_up() const
{
    if (!is_started())
    {
        return false;
    }
    std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
    return current_time >= _timer_end;

}

std::string TimerCometStage::to_string() const
{
    std::ostringstream oss;
    int time_left = get_time_left();
    if (time_left == -1)
    {
        return "Timer not started.";
    }
    if (time_left == 0)
    {
        return "Time's up!";
    }
    int minutes = time_left / 60;
    int seconds = time_left % 60;
    oss << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0') << seconds;
    return oss.str();
}

void TimerCometStage::start()
{
    Stage::start();
    _timer_end = std::chrono::steady_clock::now() + std::chrono::seconds(_timer_seconds);
}

void TimerCometStage::restart()
{
    Stage::restart();
    _timer_end = std::chrono::steady_clock::now() + std::chrono::seconds(_timer_seconds);
}