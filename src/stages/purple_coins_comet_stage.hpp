#include "comet_stage.hpp"

class PurpleCoinsCometStage : public CometStage
{
    public:

        PurpleCoinsCometStage(int total_purple_coins, int purple_coins_requirement);
        PurpleCoinsCometStage(int total_purple_coins); // same requirement as total

        int get_total_purple_coins() const;
        int get_purple_coins_requirement() const;
        int get_current_purple_coins() const;

    private:

        int _current_purple_coins;
        int _total_purple_coins;
        int _purple_coins_requirement;
};