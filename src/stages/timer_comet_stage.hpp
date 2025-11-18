#include "comet_stage.hpp"

#include <chrono>
#include <string>

class TimerCometStage : public CometStage
{
    public:

        TimerCometStage(int timer_seconds);

        int get_time_left() const;
        bool is_timer_up() const;
        std::string to_string() const;

        virtual void start() override;
        virtual void restart() override;

    private:

            std::chrono::steady_clock::time_point _timer_end;
            int _timer_seconds;

};